#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
doxygen2qtcreator.py : 

This script scans for documented classes inside Doxygen 'html' directory and inserts markers 
used by Qt Creator to generate the tooltip when hovering over a class or method name.

It uses BeautifulSoup4 to parse and modify the html files. 
"""
from __future__ import print_function
import os, sys
import argparse
from bs4 import BeautifulSoup, Comment


# Qt Creator markers 

class_brief_start = lambda classname: Comment(" $$$ %s-brief " % classname)
class_brief_end   = lambda classname: Comment(" @@@%s " % classname)
method_start = lambda methodname, signature: Comment(" $$$%s[overload1]$$$%s " % (methodname, signature))
method_end = lambda methodname: Comment(" @@@%s " % methodname)


def insert_class_markers(soup):
    """ Inserts Qt Creator markers around the class brief paragraph."""

    # look for class name in a div like <div class="title">Namespace::MyClass Class Reference</div>
    title_div = soup.find("div", "title")
    if not title_div:
        raise ValueError("The following div was not found : <div class='title'>...<div>")

    # titlediv.get_text() --> "Namespace::MyClass Class Reference"
    fullclassname = title_div.get_text().split()[0]
    classname = fullclassname.split("::")[-1]

    # look for the contents div
    contents_div = soup.find( "div", "contents" )
    if not contents_div:
        raise ValueError("The following div was not found : <div class='contents'>...<div>")
    
    # insert Qt Creator markers around the brief paragraph
    brief_p = contents_div.p
    brief_p.insert_before(class_brief_start(classname))
    brief_p.insert_after(class_brief_end(classname))


def insert_methods_markers(soup):
    """ Inserts Qt Creator markers around method docs.

    In order to display the method signature inside Qt Creator's tooltip, 
    the method signature is also inserted as a text inside a hidden div.
    """
    # look for all memitem divs
    divs = soup.find_all("div", "memitem")
    for div in divs:

        # look for method name and signature 
        memdoc = div.find("div", "memdoc")
        memname_td = div.find("td","memname")
        memname_table = div.find("table", "memname")
        if not memdoc or not memname_td or not memname_table:
            continue

        # extract method name
        fullmethodname = memname_td.get_text().strip()
        methodname = fullmethodname.split("::")[-1]

        # extract method signature
        signature = memname_table.get_text().replace("\n", "")
        compactsignature = signature.replace(" ","")
        
        # insert Qt Creator markers around the memdoc div
        memdoc.insert_before(method_start(methodname, compactsignature))
        memdoc.insert_after(method_end(methodname))

        # create a hidden div and insert method signature
        hiddendiv = soup.new_tag("div", style="display: none")
        hiddendiv.string = signature
        memdoc.insert(0, hiddendiv)


def process(filename, output):
    # open html file with BeautifulSoup
    with open(filename, encoding="utf-8") as htmlfile:
        soup = BeautifulSoup(htmlfile, "html.parser")

        # insert Qt brief markers
        insert_class_markers(soup)
        insert_methods_markers(soup)

        # write output
        with open(output, "wb") as out:
            out.write(str(soup).encode("utf-8"))    


def is_class_file(filename): 
    return filename.endswith("html") \
       and filename.startswith("class") \
       and not filename.count("members") \
       and not filename == "classes.html"


# Main
def main(htmldir, outdir, filenames, quiet):
    # if no filename was specified loop over files in htmldir
    if not filenames:
        if not quiet: print("Looking for html class files in:\n %s" % htmldir )
        filenames = [f for f in os.listdir(htmldir) if is_class_file(f)]
    
    for filename in filenames:
        if not quiet: print("Processing", filename, "...", end=" ")
        try:
            process(os.path.join(htmldir, filename), os.path.join(outdir, os.path.basename(filename)))
        except ValueError as e:
            print("Error:", e.strerror, "(File: %s)" % filename)
        except:
            print("Error:", sys.exc_info()[1], "(File: %s)" % filename)
        else:
            if not quiet: print("OK")


if __name__ == "__main__":
    
    # create the argument parser 
    parser = argparse.ArgumentParser(description="Inserts Qt Creator markers into Doxygen-generated html docs.")
    
    # main arguments
    parser.add_argument("htmldir", nargs="?",  help="Doxygen html directory (defaults to current directory)")
    # additional arguments
    parser.add_argument("-o", "--outdir", help="output directoy (defaults to htmldir)")
    parser.add_argument("-f", "--file", action="append", help="process only the specified file(s) (htmldir will be ignored)")
    parser.add_argument("-q", "--quiet", action="store_true", help="decreases output verbosity (shows only errors)")

    # parse script's arguments 
    args = parser.parse_args()
    htmldir = args.htmldir or os.getcwd()
    outdir = args.outdir or htmldir
    
    # main function    
    main(htmldir, outdir, args.file, args.quiet)

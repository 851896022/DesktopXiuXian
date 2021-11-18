#!/usr/bin/env python
# -*- coding: utf-8 -*-
# by vellhe 2017/7/9
#pi@raspberrypi:/etc/mysql/mariadb.conf.d $ sudo mysqld_safe --skip-grant-tables


from flask import Flask, abort, request, jsonify
import MySQLdb
import demjson
import time
import datetime
from HTMLTable import (
    HTMLTable,
)
from Crypto.Cipher import AES
import base64
from binascii import b2a_hex, a2b_hex
import json

import re
 
 
def find_chinese(file):
    pattern = re.compile(r'[^\u4e00-\u9fa5]')
    chinese = re.sub(pattern, '', file)
    print(chinese)
 
def find_unchinese(file):
    pattern = re.compile(r'[\u4e00-\u9fa5]')
    unchinese = re.sub(pattern,"",file)
    print(unchinese)



import hashlib
app = Flask(__name__)

# 测试数据暂时存放
tasks = []

def ftos(value):
    return "{:3.2g}".format(value)

# 解密后，去掉补足的空格用strip() 去掉
def decrypt(text):
    m = hashlib.md5()
    key = "c0e2fdbe"+time.strftime("%Y%m%d", time.localtime())
    mode = AES.MODE_ECB
    cryptos = AES.new(key, mode)
    plain_text = cryptos.decrypt(text)
    return bytes.decode(plain_text).rstrip('\0')


@app.route('/updata-new', methods=['POST'])
def add_task():
    #print("xia↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓plain_text")
    data=request.get_data(as_text=True);
    aes_json = base64.b64decode(data)
    str_json=decrypt(aes_json)
    #print(len(str_json))
    #print(str_json)
    opj_json = demjson.decode(str_json)
    #opj_json = demjson.decode('{"fangyu":181682.828902898,"gongji":191284.65611981717,"jingjie":4,"shengming":1898749.14034942,"userid":"81b52ee6b54b9c7f3fe5e408568a142f","username":"n5012346","wuxing":21650.4,"xiuwei":1888562298.745311}',encoding='utf8')
    #print(opj_json)
    # 打开数据库连接
    db = MySQLdb.connect("127.0.0.1", "root", "123", "xiuxian", charset='utf8mb4' ,port=2335)
    # 使用cursor()方法获取操作游标 
    cursor = db.cursor()
    
    # SQL 插入语句
    sql = "REPLACE INTO xiuxian_new_20210907 (uuid,user_name,sheng_ming,gong_ji,fang_yu,wu_xing,xiu_wei,jing_jie,remote_addr,up_dt) VALUES( "
    sql += " '"+opj_json['userid']+"',"
    sql += " '"+(str(opj_json['username']))[0:6] +"',"
    sql += " "+str(opj_json['shengming'])+","
    sql += " "+str(opj_json['gongji'])+","
    sql += " "+str(opj_json['fangyu'])+","
    sql += " "+str(opj_json['wuxing'])+","
    sql += " "+str(opj_json['xiuwei'])+","
    sql += " "+str(opj_json['jingjie'])+","
    sql += " '"+str(request.remote_addr)+"',"
    sql += " '"+datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')+"');"
    try:
        # 执行sql语句
        cursor.execute(sql)
        # 提交到数据库执行
        db.commit()
        
    except:
        # Rollback in case there is any error
        print(sql)
        db.rollback()
        

    return jsonify({'result': 'success'})

@app.route('/updata-chuangdangjianghu', methods=['POST'])
def add_chuangdang():
    #print("xia↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓plain_text")
    data=request.get_data(as_text=True);
    aes_json = base64.b64decode(data)
    str_json=decrypt(aes_json)
    #print(len(str_json))
    #print(str_json)
    opj_json = demjson.decode(str_json)
    #opj_json = demjson.decode('{"fangyu":181682.828902898,"gongji":191284.65611981717,"jingjie":4,"shengming":1898749.14034942,"userid":"81b52ee6b54b9c7f3fe5e408568a142f","username":"n5012346","wuxing":21650.4,"xiuwei":1888562298.745311}',encoding='utf8')
    #print(opj_json)
    # 打开数据库连接
    db = MySQLdb.connect("127.0.0.1", "root", "123", "xiuxian", charset='utf8mb4' ,port=2335)
    # 使用cursor()方法获取操作游标 
    cursor = db.cursor()
    
    # SQL 插入语句
    sql = "INSERT INTO `chuangdang`( `uuid`, `uname`, `jianggong`, `jifen`, `updt`) VALUES ("

    sql += " '"+opj_json['userid']+"',"
    sql += " '"+(str(opj_json['uname']))[0:6] +"',"
    sql += " "+str(opj_json['jianggong'])+","
    sql += " "+str(opj_json['jifen'])+","
    sql += " '"+datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')+"');"
    try:
        # 执行sql语句
        cursor.execute(sql)
        # 提交到数据库执行
        db.commit()
        
    except:
        # Rollback in case there is any error
        print(sql)
        db.rollback()
        

    return jsonify({'result': 'success'})

@app.route('/updata', methods=['GET'])
def get_updata():
    return '这个服务是使用Python编写运行在树莓派上的，脆弱得很，请大佬高抬贵手'

@app.route('/', methods=['GET'])
def get_root():
    return '这个服务是使用Python编写运行在树莓派上的，脆弱得很，请大佬高抬贵手'

@app.route('/feedback', methods=['POST'])
def post_feedback():
    data=request.get_data(as_text=True)
    db = MySQLdb.connect("127.0.0.1", "root", "123", "xiuxian", charset='utf8mb4' ,port=2335)
    # 使用cursor()方法获取操作游标 
    cursor = db.cursor()
    
    # SQL 插入语句
    sql = "REPLACE INTO feedback (info,up_dt) VALUES( "
    sql += " '"+str(data)+"',"
    sql += " '"+datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')+"');"
    print (sql)
    try:
        # 执行sql语句
        cursor.execute(sql)
        # 提交到数据库执行
        db.commit()
        
    except:
        # Rollback in case there is any error
        print(sql)
        db.rollback()
        

    return jsonify({'result': 'success'})

@app.route('/lastVersion', methods=['GET'])
def get_version():
    return '''1.5.1.0|https://www.52pojie.cn/thread-1498266-1-1.html|
    <h1>开启全新资料片【闯荡江湖】</h1><br>
    1、擂台积分排名上线<br>
    2、大家可以更和谐地论剑了

    '''


@app.route('/rank/xiuwei', methods=['GET'])
def get_rank_xiuwei():
    # 标题
    table = HTMLTable(caption='')
    # 数据行
    table.append_data_rows((
        ('排名','道号', '生命', '攻击', '防御','修为','境界'),
    ))
    # 打开数据库连接
    db = MySQLdb.connect("127.0.0.1", "root", "123", "xiuxian", charset='utf8mb4' ,port=2335)
    # 使用cursor()方法获取操作游标 
    cursor = db.cursor()
    
    # SQL 插入语句
    sql = """
        SELECT user_name,sheng_ming,gong_ji,fang_yu,xiu_wei,jingjie_str,uuid,id
        FROM `xiuxian_new_20210907` a  join `dict_jingjie` b on a.jing_jie = b.jingjie_id 
        WHERE LENGTH(user_name)>1 
				and sheng_ming <1e30  AND sheng_ming > 100000
				AND gong_ji < 1e30 
				AND fang_yu < 1e30 
				AND xiu_wei<4.87934578929732e29 
				AND HOUR( timediff( now(), up_dt) ) < 1
				AND (id IN (select max(id) from xiuxian_new_20210907   group by uuid))
				AND sheng_ming <> gong_ji
				AND gong_ji <> fang_yu
				AND fang_yu <> xiu_wei
				AND xiu_wei <> sheng_ming
				AND fang_yu <> sheng_ming
				
        ORDER BY xiu_wei desc LIMIT 0,200 ;
        """
    try:
        # 执行sql语句
        cursor.execute(sql)
        # 提交到数据库执行
        #
        db.commit()
        # 获取所有记录列表
        results = cursor.fetchall()
        i=0
        for row in results:
            i=i+1
            table.append_data_rows((
                (str(i),str(row[0]),ftos(row[1]),ftos(row[2]),ftos(row[3]),ftos(row[4]),str(row[5])),
                ))
    except:
        # Rollback in case there is any error
        print(sql)
        db.rollback()
    table.set_cell_style({
    'border-color': '#000',
    'border-width': '1px',
    'border-style': 'solid',
    'padding': '5px',
    })
    html = table.to_html()
    return html
@app.route('/rank/shengming', methods=['GET'])
def get_rank_shengming():
    # 标题
    table = HTMLTable(caption='')
    # 数据行
    table.append_data_rows((
        ('道号', '生命', '攻击', '防御','修为','境界'),
    ))
    # 打开数据库连接
    db = MySQLdb.connect("127.0.0.1", "root", "123", "xiuxian", charset='utf8mb4' ,port=2335)
    # 使用cursor()方法获取操作游标 
    cursor = db.cursor()
    
    # SQL 插入语句
    sql = """
        SELECT user_name,sheng_ming,gong_ji,fang_yu,xiu_wei,jingjie_str,uuid,id
        FROM `xiuxian_new_20210907` a  join `dict_jingjie` b on a.jing_jie = b.jingjie_id 
        WHERE LENGTH(user_name)>1 
				and sheng_ming <1e30 
				AND gong_ji < 1e30 
				AND fang_yu < 1e30 
				AND xiu_wei<4.87934578929732e29 
				AND HOUR( timediff( now(), up_dt) ) < 24
				AND (id IN (select max(id) from xiuxian_new_20210907   group by uuid))
				AND sheng_ming <> gong_ji
				AND gong_ji <> fang_yu
				AND fang_yu <> xiu_wei
				AND xiu_wei <> sheng_ming
				AND fang_yu <> sheng_ming
        ORDER BY sheng_ming desc LIMIT 0,20 ;
        """
    try:
        # 执行sql语句
        cursor.execute(sql)
        # 提交到数据库执行
        #
        db.commit()
        # 获取所有记录列表
        results = cursor.fetchall()
        for row in results:
            table.append_data_rows((
                (str(row[0]),ftos(row[1]),ftos(row[2]),ftos(row[3]),ftos(row[4]),str(row[5])),
                ))
    except:
        # Rollback in case there is any error
        print(sql)
        db.rollback()
    table.set_cell_style({
    'border-color': '#000',
    'border-width': '1px',
    'border-style': 'solid',
    'padding': '5px',
    })
    html = table.to_html()
    return html
@app.route('/rank/gongji', methods=['GET'])
def get_rank_gongji():
    # 标题
    table = HTMLTable(caption='')
    # 数据行
    table.append_data_rows((
        ('道号', '生命', '攻击', '防御','修为','境界'),
    ))
    # 打开数据库连接
    db = MySQLdb.connect("127.0.0.1", "root", "123", "xiuxian", charset='utf8mb4' ,port=2335)
    # 使用cursor()方法获取操作游标 
    cursor = db.cursor()
    
    # SQL 插入语句
    sql = """
        SELECT user_name,sheng_ming,gong_ji,fang_yu,xiu_wei,jingjie_str,uuid,id
        FROM `xiuxian_new_20210907` a  join `dict_jingjie` b on a.jing_jie = b.jingjie_id 
        WHERE LENGTH(user_name)>1 
				and sheng_ming <1e30 
				AND gong_ji < 1e30 
				AND fang_yu < 1e30 
				AND xiu_wei<4.87934578929732e29 
				AND HOUR( timediff( now(), up_dt) ) < 24
				AND (id IN (select max(id) from xiuxian_new_20210907   group by uuid))
				AND sheng_ming <> gong_ji
				AND gong_ji <> fang_yu
				AND fang_yu <> xiu_wei
				AND xiu_wei <> sheng_ming
				AND fang_yu <> sheng_ming
        ORDER BY gong_ji desc LIMIT 0,20 ;
        """
    try:
        # 执行sql语句
        cursor.execute(sql)
        # 提交到数据库执行
        #
        db.commit()
        # 获取所有记录列表
        results = cursor.fetchall()
        for row in results:
            table.append_data_rows((
                (str(row[0]),ftos(row[1]),ftos(row[2]),ftos(row[3]),ftos(row[4]),str(row[5])),
                ))
    except:
        # Rollback in case there is any error
        print(sql)
        db.rollback()
    
    table.set_cell_style({
    'border-color': '#000',
    'border-width': '1px',
    'border-style': 'solid',
    'padding': '5px',
    })
    
    html = table.to_html()
    return html
@app.route('/rank/fangyu', methods=['GET'])
def get_rank_fangyu():
    # 标题
    table = HTMLTable(caption='')
    # 数据行
    table.append_data_rows((
        ('道号', '生命', '攻击', '防御','修为','境界'),
    ))
    # 打开数据库连接
    db = MySQLdb.connect("127.0.0.1", "root", "123", "xiuxian", charset='utf8mb4' ,port=2335)
    # 使用cursor()方法获取操作游标 
    cursor = db.cursor()
    
    # SQL 插入语句
    sql = """
        SELECT user_name,sheng_ming,gong_ji,fang_yu,xiu_wei,jingjie_str,uuid,id
        FROM `xiuxian_new_20210907` a  join `dict_jingjie` b on a.jing_jie = b.jingjie_id 
        WHERE LENGTH(user_name)>1 
				and sheng_ming <1e30 
				AND gong_ji < 1e30 
				AND fang_yu < 1e30 
				AND xiu_wei<4.87934578929732e29 
				AND HOUR( timediff( now(), up_dt) ) < 24
				AND (id IN (select max(id) from xiuxian_new_20210907   group by uuid))
				AND sheng_ming <> gong_ji
				AND gong_ji <> fang_yu
				AND fang_yu <> xiu_wei
				AND xiu_wei <> sheng_ming
				AND fang_yu <> sheng_ming
        ORDER BY fang_yu desc LIMIT 0,20 ;
        """
    try:
        # 执行sql语句
        cursor.execute(sql)
        # 提交到数据库执行
        #
        db.commit()
        # 获取所有记录列表
        results = cursor.fetchall()
        for row in results:
            table.append_data_rows((
                (str(row[0]),ftos(row[1]),ftos(row[2]),ftos(row[3]),ftos(row[4]),str(row[5])),
                ))
    except:
        # Rollback in case there is any error
        print(sql)
        db.rollback()
    table.set_cell_style({
    'border-color': '#000',
    'border-width': '1px',
    'border-style': 'solid',
    'padding': '5px',
    })
    html = table.to_html()
    return html

@app.route('/rank/jifen', methods=['GET'])
def get_rank_jifen():
    # 标题
    table = HTMLTable(caption='')
    # 数据行
    table.append_data_rows((
        ('排名','道号', '积分'),
    ))
    # 打开数据库连接
    db = MySQLdb.connect("127.0.0.1", "root", "123", "xiuxian", charset='utf8mb4' ,port=2335)
    # 使用cursor()方法获取操作游标 
    cursor = db.cursor()
    
    # SQL 插入语句
    sql = """
        SELECT uname,jianggong,jifen,uuid,ziznegid
        FROM `chuangdang`
        WHERE HOUR( timediff( now(), updt) ) < 24
				AND (ziznegid IN (select max(ziznegid) from chuangdang   group by uuid))
        ORDER BY jifen desc LIMIT 0,200 ;
        """
    try:
        # 执行sql语句
        cursor.execute(sql)
        # 提交到数据库执行
        #
        db.commit()
        # 获取所有记录列表
        results = cursor.fetchall()
        i=1
        for row in results:
            table.append_data_rows((
                (str(i),str(row[0]),str(row[2])),
                ))
            i=i+1
    except:
        # Rollback in case there is any error
        print(sql)
        db.rollback()
    table.set_cell_style({
    'border-color': '#000',
    'border-width': '1px',
    'border-style': 'solid',
    'padding': '5px',
    })
    html = table.to_html()
    return html


if __name__ == "__main__":
    # 将host设置为0.0.0.0，则外网用户也可以访问到这个服务
    app.run(host="0.0.0.0", port=8522)


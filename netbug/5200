import requests
import random
import bs4
import os
import re

def get_header():
    agent = ['Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:59.0) Gecko/20100101 Firefox/59.0',
             'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36',
             'Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko',
             'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.221 Safari/537.36 SE 2.X MetaSr 1.0',
             'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.89 Safari/537.36',
             'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.139 Safari/537.36'
             ]
    i = random.randint(0, len(agent) - 1)
    header = {'User-Agent': agent[i]}
    return header

def get_html(url):
    head=get_header()
    req=requests.get(url,head)
    return req.text

def get_list(html):
    url_list=[]
    soup=bs4.BeautifulSoup(html,'html.parser')
    global nov_name
    nov_name=soup.h1.text
    root_url="https://www.5200.net"
    total_list=soup.find_all("dd")
    for each in total_list:
        each_url=root_url+each.a['href']
        url_list.append(each_url)
    return url_list

def get_content(url_list):
    size=len(url_list)
    print(f'抓取到小说：{nov_name}共{size}章节！')
    if not os.path.exists("D:/1/noval"):
        os.makedirs("D:/1/noval")
    k=0
    for each_url in url_list:
        k+=1
        each_html=get_html(each_url)
        each_soup=bs4.BeautifulSoup(each_html,'html.parser')
        each_name=each_soup.h1.text
        txt=each_soup.find_all('div',attrs={"id":"content"})
        spt=re.split("\s+", txt[0].text)
        this=[]
        this.append(str('    '+each_name+"\n\n"))
        for i in spt:
            i = '    ' + i + "\n\n"
            this.append(i)

        print(f'正在写入第{k}/{size}章:{each_name} ^^')
        '''with open("D:/1/noval/" + each_name + ".txt", 'a', encoding = 'utf-8')as f:
            f.writelines(this)'''
        # total noval
        with open("D:/1/noval/" + nov_name + ".txt", 'a+', encoding = 'utf-8')as p:
            p.writelines(this)

if __name__=="__main__":
    url = input("请输入:https://www.5200.net中最任意一小说目录所在网址(例如：https://www.5200.net/0_20/)：")
    html=get_html(url)
    list_all=get_list(html)
    con=get_content(list_all)

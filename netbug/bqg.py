import os
import re
import requests
import bs4
import random

class nov():
    def __init__(self,url):
        self.url=url

    def get_header(self):
        agent = ['Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:59.0) Gecko/20100101 Firefox/59.0',
             'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36',
             'Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko',
             'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.221 Safari/537.36 SE 2.X MetaSr 1.0',
             'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.89 Safari/537.36',
             'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.139 Safari/537.36'
             ]
        i = random.randint(0,len(agent)-1)
        header = {'User-Agent':agent[i]}
        return header

    # 获取目录页面
    def get_html(self,path):
        self.path=path
        res=requests.get(self.path,self.get_header())
        res.encoding='utf-8'
        html=res.text
        return html

    #获取章节全路径
    def get_each(self):
        self.html=self.get_html(self.url)
        soup=bs4.BeautifulSoup(self.html,'html.parser')
        global nov_name
        nov_name=soup.h1.text
        each_path=soup.find_all('dd')
        urlList=[]
        for i in each_path:
            each_name=i.a.text
            each_full_path='http://www.xbiquge.la'+i.a['href']
            urlList.append(each_full_path)
        return urlList

    def get_content(self):
        self.ulist=self.get_each()
        print("匹配到小说:<<"+nov_name+">>共%d章节" % len(self.ulist))

        down="D:/1/txt/"
        if os.path.exists(down)!=True:
            os.makedirs(down)
        k=0
        for i in self.ulist[:]:
            k+=1
            con=self.get_html(i)
            soup=bs4.BeautifulSoup(con,'html.parser')
            title=soup.h1.text
            txt=soup.find_all('div',id="content")
            spt=re.split("\s+",txt[0].text)
            new=[]
            new.append(str('    '+title+"\n\n"))
            for i in spt:
                i='    '+i+"\n\n"
                new.append(i)

            print("正在写入第%d章" % k)
            with open("D:/1/txt/"+nov_name+".txt",'a+',encoding='utf-8')as p:
                p.writelines(new)

if __name__=="__main__":
    www="http://www.xbiquge.la"
    url=input("请输入:"+str(www)+"中最任意一小说目录所在网址\n(例如：http://www.xbiquge.la/15/15409/)：")
    n=nov(url)
    n.get_html(url)
    n.get_each()
    n.get_content()

# 关于DBLP的一些信息
原文：[dblpxml.pdf](https://dblp.uni-trier.de/xml/docu/dblpxml.pdf)。  
最后更新时间：2020年3月25日。  

## DBLP记录
DBLP数据集可从[https://dblp.uni-trier.de/xml/](https://dblp.uni-trier.de/xml/)位置获得。  
文件`dblp.xml`包含所有的索引记录，它的格式通过`dblp.dtd`文件定义。`dblp.xml`的大致结构如下：  

```xml
<?xml version="1.0" encoding="ISO-8859-1"?>
<!DOCTYPE dblp SYSTEM "dblp.dtd">
<dblp>
    record 1
    ...
    record n
</dblp>
```

第一行指定了文件编码为ISO-8859-1（“Latin-1”），但实际上文件中只包含纯粹的ASCII字符。所有非ASCII字符都是用实体名称或实体数字表示。例如`&eacute;`（定义在`dblp.dtd`文件中）或者`&#233;`用来表示字符&#233;。

XML的根元素`<dblp>`包含了一长串的索引记录，在DTD文件中列出了可能的记录名：

```dtd
<!ELEMENT dblp (article|inproceedings|proceedings|book|incollection|
                phdthesis|mastersthesis|www|person|data)*>
```

这些标签实际上对应了bibtex的文章类型。  
  
|     标签名     |         含义         |
| ------------- | -------------------- |
| article       | 期刊杂志的论文        |
| book          | 公开出版的图书        |
| incollection  | 书籍中带独立标题的章节 |
| inproceedings | 会议论文集中的一篇     |
| phdthesis     | 博士论文              |
| proceedings   | 会议论文集            |
| mastersthesis | 硕士论文              |

## 记录属性
例如：

```xml
<article key="journals/cacm/Szalay08" 
         mdate="2008-11-03">
    <author>Alexander S. Szalay</author>
    <title>Jim Gray, astronomer.</title>
    <pages>58-65</pages>
    <year>2008</year>
    <volume>51</volume>
    <journal>Commun. ACM</journal>
    <number>11</number>
    <ee>http://doi.acm.org/10.1145/1400214.1400231</ee>
    <url>db/journals/cacm/cacm51.html#Szalay08</url>
</article>
```

这条记录描述了一篇来自CACM （Communications of the ACM）的文章，`article`标签有两个属性：  

* **key** 是一条记录的唯一关键码。关键码看上去就像一个用斜杠分割的Unix文件名。例如`conf/*`表示会议论文或研讨会论文，`journals/*`表示发布在期刊（journals）、通讯期刊（transactions）、杂志（magazines）或者简讯（newsletters）上的文章。关键码的第二部分通常是会议名或者期刊名。最后一部分是一串任意的字母和数字组成的序列（通常从作者名和发表年份生成，有时会添加一些字母来保持关键码唯一）。注意：当对记录信息进行修改的时候，关键码是不会变化的！
* **mdate**是这条记录最后一次修改的日期，符合ISO 8601格式，即`YYYY-MM-DD`。我们并没有专门记录dblp的变更日志，但是通过mdate属性可以很容易读取到一条记录的变更情况。`dblp.xml`文件是不断扩展的，即使记录的信息变更了，旧有的信息也不会被删除。

## 作者
在BibTex中，最多只有一个author区域，包含一个作者列表。但在dblp中，每个作者对应一个author标签，标签的顺序非常重要，和论文上的署名顺序一致。


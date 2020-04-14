#include "parser.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QDataStream>
#include <QTime>
#include <QDebug>
#include <QMap>
#include <QList>

QString Parser::s_yearword[70];

Parser::Parser(QObject *parent)
    :QThread(parent)
{
    
}

void Parser::run()
{
    parse();
}

void Parser::parse()
{
    QTime timing;
    timing.start();
    emit stateChanged(tr("Parsing start."));
    int elapsedTime = 0;
    
    QFile file;
    QTextStream textStream(&file);
    QDataStream dataStream(&file);
    
    StringRef::init(Util::getXmlFileName());
    quint32 len = StringRef::s_len;
    
    StringRef ref(0, len);
    file.close();
    m_costMsecs = timing.elapsed();
    emit stateChanged(tr("XML file read successful. (%1 ms)").arg(m_costMsecs - elapsedTime));
    elapsedTime = m_costMsecs;
    QVector<StringRef> authorIndex;
    QVector<StringRef> titleIndex;
    quint32 x = 0;
    int totalAuthor = 0;
    // authorId starts from 0
    QMap<StringRef, QPair<int/*id*/, int/*stac*/>> authorInfo;
    QVector<StringRef> authors;
    QVector<QVector<int>> authorsIdRelation;

    quint32 titlelen = 0;
    QString temp1,temp2;
    QMap<QString,int> yearwordsTemp[70];
    QString commonwords = "a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,an,are,all,any,been,both,each,either,one,two,three,four,five,"
                          "six,seven,eight,nine,ten,none,little,few,many,much,other,another,some,no,every,nobody,anybody,somebody,everybody,"
                          "when,on,under,as,first,second,third,forth,fifth,sixth,seventh,above,over,below,under,beside,behind,of,the,after,from,"
                          "since,for,which,by,next,where,how,who,there,is,was,were,do,did,done,this,that,in,last,brfore,because,against,except,beyond,"
                          "along,among,but,so,towards,to,it,me,he,you,your,his,her,she,its,they,them,and,has,have,had,my,would,then,too,or,our,off,or,we,be,"
                          "into,weel,can,being,been,having,even,us,these,those,if,ours,with,use,using,used,the,based,-,:,;,problem,problems,systems,methods,"
                          "ways,ideas,learning,information,works,solve,solving,solved,old,new,analysis,data,big,small,large,their,,between,0,1,2,3,4,5,6,7,8,9";

    while (x < len){
        if (ref.startsWith("key=\"", x)) {
            x += 5;
            QVector<int> recordAuthorsId;

            StringRef xxxxx;

            while (x <= len) {
                if (x == len || ref.startsWith("key=\"", x + 1)) {
                    if (recordAuthorsId.size() > 1) {
                        authorsIdRelation.append(recordAuthorsId);
                    }
                    break;
                }
                if (ref[x] == '<') {
                    if (ref.startsWith("author", x + 1)) {
                        StringRef author = readElementText(ref, x);
                        QPair<int, int> *info;
                        if (authorInfo.contains(author)) {
                            info = &authorInfo[author];
                        } else {
                            info = &authorInfo[author];
                            info->first/*id*/ = totalAuthor;
                            ++totalAuthor;
                            authors.append(author);
                        }
                        ++info->second;
                        authorIndex.append(author);
                        recordAuthorsId.append(info->first/*id*/);
//                        qDebug() << author;
                    } else if (ref.startsWith("title", x + 1)) {
                        StringRef title = readElementText(ref, x);
                        titleIndex.append(title);

                        xxxxx = title;
//                        QStringList tem = title.toString().split(QRegExp("[ .,*/^:;()!?@&]"));
//                        QString temz = QString::number(titlelen) + ",";

                        
//                        qDebug() << title;
                    }else if (ref.startsWith("year", x + 1)) {
                        StringRef year = readElementText(ref, x);
                        int yearyy= year.toString().toInt() - 1950;
                        if(yearyy <70 && yearyy >= 0)
                        {
                            StringRef temy = xxxxx;
                            QStringList tem = temy.toString().split(QRegExp("[ .,*/^:;()!?@&]"));

                            QStringListIterator strIterator(tem);
                            while(strIterator.hasNext())
                            {
                                temp1 = strIterator.next();
                                temp1 = temp1.toLower();
                                if(temp1 != "" && commonwords.contains(temp1) == 0)
                                    ++yearwordsTemp[yearyy][temp1];
                            }
                        }
//                        qDebug() << year;
                    }
                }
                ++x;
            }
        }
        ++x;
    }
    m_costMsecs = timing.elapsed();
    emit stateChanged(tr("XML file parse successful. (%1 ms)").arg(m_costMsecs - elapsedTime));
    elapsedTime = m_costMsecs;

    // Save authors to authors.txt
    // The author's ID in line x is x
    
    file.setFileName("authors.txt");
    file.open(QFile::WriteOnly | QFile::Text);
    foreach (StringRef author, authors) {
        textStream << author.toString() << '\n';
    }
    file.close();
    
    // Save authors relation to authors_relation.txt
    file.setFileName("authors_relation.txt");
    file.open(QFile::WriteOnly | QFile::Text);
    textStream << totalAuthor << '\n';
    for (auto relation : authorsIdRelation) {
        textStream << relation[0];
        for (int i = 1; i < relation.size(); ++i) {
            textStream << ' ' << relation[i];
        }
        textStream << '\n';
    }
    file.close();
    
    m_costMsecs = timing.elapsed();
    emit stateChanged(tr("Authors information saved. (%1 ms)").arg(m_costMsecs - elapsedTime));
    elapsedTime = m_costMsecs;

    QList<QPair<StringRef, int>> temp;
    temp.reserve(authorInfo.size());

    auto it=authorInfo.begin();
    while (it != authorInfo.end()) {
        temp.append(qMakePair(it.key(),it.value().second));
        it++;
    }

    std::sort(temp.begin(),temp.end(),sortByDesc);

    QVector<QPair<QString, int>> authorStac;
    for (qint32 t=0; t<temp.size(); t++) {
        authorStac.append(qMakePair(temp[t].first.toString(), temp[t].second));
    }

    std::sort(authorIndex.begin(), authorIndex.end());
    std::sort(titleIndex.begin(), titleIndex.end());

    QList<QPair<QString,int> > temz;
    QMap<QString, int>::iterator its;
    for(int i=0;i<70;i++)
    {

        if(yearwordsTemp[i].begin().key()==nullptr)continue;
        its=yearwordsTemp[i].begin();
        while(its!=yearwordsTemp[i].end())
        {
            temz.append(qMakePair(its.key(),its.value()));
            its++;
        }
        yearwordsTemp[i].clear();
        std::sort(temz.begin(),temz.end(),sortByYear);
        int num1 = temz.size()<=10 ? temz.size() : 10;
        for(qint32 t=0;t<num1;t++)
            s_yearword[i] = s_yearword[i] + temz[t].first + ",";
        temz.clear();
        qDebug()<<i;
    }
    for(int i = 0; i < 70; i++)
        qDebug() << i+1950 << ":" << s_yearword[i];

    
    m_costMsecs = timing.elapsed();
    emit stateChanged(tr("Index file generated. (%1 ms)").arg(m_costMsecs - elapsedTime));
    elapsedTime = m_costMsecs;
    
    StringRef::clean();
    
    file.setFileName("author.dat");
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    foreach(auto i, authorIndex){
        dataStream << i.l << i.r;
    }
    file.close();
    
    file.setFileName("title.dat");
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    foreach(auto i, titleIndex){
        dataStream << i.l << i.r;
    }
    file.close();

    file.setFileName("authorStac.dat");
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    int num = authorStac.size()<=100 ? authorStac.size() : 100;
    for(int i=0;i<num;i++){
        dataStream << authorStac[i].first << authorStac[i].second;
    }
    file.close();

    m_costMsecs = timing.elapsed();
    emit stateChanged(tr("Index file saved. (%1 ms)").arg(m_costMsecs - elapsedTime));
    emit stateChanged(tr("Parse done. Cost time: %1").arg(Util::formatTime(m_costMsecs)));
    qInfo() << QString("Parse done in %1 ms").arg(m_costMsecs);
    emit done();
}

StringRef Parser::readElementText(const StringRef &r, quint32 &from)
{
    StringRef s = r.mid(from);
    Q_ASSERT(s[0] == '<');
    quint32 i = 1;
    char name[30];
    name[0] = '<';
    name[1] = '/';
    while(s[i] != ' ' && s[i] != '>'){
        name[i + 1] = s[i];
        ++i;
    }
    from += i;
    name[i + 1] = '>';
    name[i + 2] = 0;
    // name = "</ele>"
    while(s[i] != '>') ++i;
    qint32 p = s.indexOf(name, i + 1);
    Q_ASSERT(p != -1);
    quint32 x = static_cast<quint32>(p);
    from += x + 1;
    return s.mid(i + 1, x - i - 1);
}

StringRef Parser::readElementAttr(const StringRef &r, quint32 from)
{
    quint32 i = from;
    while (r[i] != '\"') ++i;
    return r.mid(from, i - from);
}

int Parser::costMsecs()
{
    return m_costMsecs;
}

void Parser::clearIndex()
{
    QFile("author.dat").remove();
    QFile("title.dat").remove();
    QFile("authorStac.dat").remove();
    QFile("authors.txt").remove();
    QFile("authors_relation.txt").remove();
    QFile("authors.edges").remove();
    QFile("cliques_count.txt").remove();
}


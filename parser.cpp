#include "parser.h"

#include <QXmlStreamReader>

Parser::Parser(QXmlStreamReader *r)
{
    reader = r;
    if(reader->device() != 0){
        while(!reader->atEnd()){
            if(reader->isStartDocument()){
                setDocumentVersion(reader->documentVersion().toString());
                setDocumentEncoding(reader->documentEncoding().toString());
                break;
            }else{
                reader->readNext();
            }
        }
    }
}

QString Parser::documentVersion() const
{
    return documentVersion_;
}

void Parser::setDocumentVersion(QString s)
{
    documentVersion_ = s;
}

QString Parser::documentEncoding() const
{
    return documentEncoding_;
}

void Parser::setDocumentEncoding(QString s)
{
    documentEncoding_ = s;
}

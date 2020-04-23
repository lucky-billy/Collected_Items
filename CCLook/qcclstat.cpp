#include "qcclstat.h"

QCCLStat::QCCLStat(QObject *parent)
	: QObject(parent)
{
}

QCCLStat::~QCCLStat()
{
}


/// ½Ó¿Ú
QList<QScoreStruct> QCCLStat::getUserScore() const
{
	QList<QScoreStruct> list;
	QFile file("score.xml");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QXmlStreamReader reader(&file);
	
	QScoreStruct* pScore;

	while(!reader.atEnd())
	{
		if (reader.isStartElement())
		{
			if (reader.name() == "User")
				pScore = new QScoreStruct;
			if (reader.name() == "Name")
				pScore->m_userName = reader.readElementText();
			if (reader.name() == "Value")
				pScore->m_userScore = reader.readElementText().toInt();
		}
		if (reader.isEndElement())
		{
			if (reader.name() == "User")
			{
				list.append(*pScore);
				delete pScore;
			}
		}
		reader.readNext();
	}
	return list;
}

void QCCLStat::setUserScore(QList<QScoreStruct> list)
{
	QFile file("score.xml");
	file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
	QXmlStreamWriter writter(&file);
	writter.setAutoFormatting(true);

	writter.writeStartDocument();
	writter.writeStartElement("Data");
	for (int i = 0; i < list.count(); i++)
	{
		writter.writeStartElement("User");
		writter.writeTextElement("Name", tr("%1").arg(list[i].m_userName));
		writter.writeTextElement("Value", tr("%1").arg(list[i].m_userScore));
		writter.writeEndElement();
	}
	writter.writeEndElement();
	writter.writeEndDocument();
}
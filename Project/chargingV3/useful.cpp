#include "charging.h"
#pragma execution_character_set("utf-8")




QString charging::get_back_message_at(QString message, int i)
{
	i = i - 1;
	QString strReturn = message;
	while (i > 0)
	{
		int n = strReturn.indexOf(",");
		if (n <= 0)return"";

		strReturn = strReturn.right(strReturn.size() - n - 1);
		i--;
	}
	int n = strReturn.indexOf(",");
	if (n >= 0)
	{
		strReturn = strReturn.left(n);
		return strReturn;
	}
	else
		return"";
}

QString charging::no_zero(QString message)
{
	if (message.at(0) == QChar('0'))message = message.right(3);
	return message;
}
 

































#include "MySQLConnection.h"
#include "UE4MySQL.h"

#include <string>


bool UMySQLConnection::MySQLCloseConnection(UMySQLConnection* Connection)
{
	if (Connection)
	{
		if (Connection->IsConnected())
		{
			mysql_close(Connection->GetConnection());
			Connection->SetConnection(nullptr);
			mysql_library_end();
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MySQLCloseConnection: Connection is valid but Server does no respond!"));
		}
	} 
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MySQLCloseConnection: Connection is null!"));
	}
	return false;
}

bool UMySQLConnection::IsConnected()
{
	return m_MySQLConnection && mysql_ping(m_MySQLConnection) == 0;
}

UMySQLConnection* UMySQLConnection::MySQLInitConnection(FString Host, FString UserName, FString UserPassword, FString DatabaseName)
{
	std::string HostString(TCHAR_TO_UTF8(*Host));
	std::string UserNameString(TCHAR_TO_UTF8(*UserName));
	std::string UserPasswordString(TCHAR_TO_UTF8(*UserPassword));
	std::string DatabaseNameString(TCHAR_TO_UTF8(*DatabaseName));

	UMySQLConnection* const MySQLConnection = NewObject<UMySQLConnection>();

	if (mysql_library_init(0, nullptr, nullptr) != 0)
	{
		UE_LOG(LogTemp, Error, TEXT("MySQLInitConnection: FAILED TO INIT mysql library"));
		mysql_library_end();
		return nullptr;
	}

	MySQLConnection->m_MySQLConnection = mysql_init(nullptr);
	if (!MySQLConnection->GetConnection())
	{
		UE_LOG(LogTemp, Error, TEXT("MySQLInitConnection: FAILED TO INIT connection"));
		mysql_library_end();
		return nullptr;
	}

	if (!mysql_real_connect(MySQLConnection->GetConnection(), HostString.c_str(), UserNameString.c_str(), UserPasswordString.c_str(), DatabaseNameString.c_str(), 0, nullptr, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("MySQLInitConnection: Failed to Connect to Database!"));
		UMySQLConnection::MySQLCloseConnection(MySQLConnection);
		return nullptr;
	}

	if (mysql_set_character_set(MySQLConnection->GetConnection(), "utf8") != 0)
	{
		UE_LOG(LogTemp, Error, TEXT("MySQLInitConnection: Can't set UTF-8 with mysql_set_character_set"));
		UMySQLConnection::MySQLCloseConnection(MySQLConnection);
		return nullptr;
	}

	return MySQLConnection;
}

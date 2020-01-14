#pragma once

#include  "mysql.h"

#include "MySQLConnection.generated.h"


UCLASS()
class UE4MYSQL_API UMySQLConnection final : public UObject
{
	GENERATED_BODY()

	MYSQL* m_MySQLConnection;

public:

	UMySQLConnection(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	/** Is the connection connected? */
	UFUNCTION(BlueprintPure, Category = "UE4MySQL")
	bool IsConnected();

	/** Get the connection. */
	FORCEINLINE MYSQL* GetConnection() const { return m_MySQLConnection; }

	/** Set the connection. */
	void SetConnection(MYSQL* NewConnection) { m_MySQLConnection = NewConnection; }

	/**
	 * Close a MYSQL connection
	 * 
	 * @param UMySQLConnection * Connection The connection that should be closed.
	 */
	UFUNCTION(BlueprintCallable, Category = "UE4MySQL", meta = (DisplayName = "MySQL Close Connection"))
	static bool MySQLCloseConnection(UMySQLConnection* Connection);

	UFUNCTION(BlueprintCallable, Category = "UE4MySQL", meta = (DisplayName = "MySQL Init Connection"))
	static UMySQLConnection* MySQLInitConnection(FString Host, FString UserName, FString UserPassword, FString DatabaseName);

};

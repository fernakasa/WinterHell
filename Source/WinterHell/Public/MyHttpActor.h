// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyHttpActor.generated.h"


// estructura para recibir (listado jugadores y veces que jugaron)

USTRUCT(BlueprintType)
struct  FResponse_PlayerList {
	GENERATED_BODY()

	UPROPERTY() FString jugadorId;
	UPROPERTY() FString nickname;
	UPROPERTY() FString cantidadJugadas;

	FResponse_PlayerList() {}
};

//USTRUCT(BlueprintType)
//struct  FResponse_TotalPlayers {
//	GENERATED_BODY()
//
//	UPROPERTY() TArray<FResponse_PlayerList> players;
//	
//
//	FResponse_TotalPlayers() {}
//};

USTRUCT(BlueprintType)
struct  FResponse_TotalPlayers {
	GENERATED_BODY()

	UPROPERTY() FString number;


	FResponse_TotalPlayers() {}
};


UCLASS()
class WINTERHELL_API AMyHttpActor : public AActor
{
	GENERATED_BODY()
	
private:	
	// unreal http implementacion, el modulo, para crear objetos request
	FHttpModule* Http;

	// ruta de la API
	FString ApiBaseUrl = "localhost:8081/api/WH/";

	// atributo que guarda el resultado de las consultas (true o false)
	bool requestSuccess;

	UPROPERTY()
	TArray<FString> PlayersNames;

	UPROPERTY()
	TArray<FString> PlayersPlays;

	FString Number;

	//template para la estructura JSON que me va a devolver la BD
	template <typename StructType>
	void GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput);

		
	// cabeza de autorizacion
	// = TEXT("User-Agent"), "X-UnrealEngine-Agent" para agentes unreal
	// = "Content-Type", TEXT("application/json" para archivos JSON
	//FString AuthorizationHeader = TEXT("Authorization");

public:	
	UFUNCTION(BlueprintCallable)
	void EmptyLists();

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetPlayersNames();

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetPlayersPlays();

	void SetPlayersNames(FString value);
	void SetPlayersPlays(FString value);
		

	// Set de requestSucces
	void SetRequestSuccess(bool value);

	// Get de requestSueccess
	// Se puede llamar desde BP
	UFUNCTION(BlueprintCallable)
	bool GetRequestSuccess();

	// Metodo para testear que el actor c++ tome un parametro del blueprint y lo devuelva (bypass)
	UFUNCTION(BlueprintCallable)
	FString TestHelloWorld(FString PlayerName);

	UFUNCTION(BlueprintCallable)
	void TestApiConn();

	void TestResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);

	UFUNCTION(BlueprintCallable)
	void InsertPlayer(FString PlayerName); 

	UFUNCTION(BlueprintCallable)
	void GetPlayersList();

	void PlayerListResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);




	UFUNCTION(BlueprintCallable)
	void GetNumberOfPlayers();
	void GetNumberOfPlayersResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);

	UFUNCTION(BlueprintCallable)
	int32 GetNumber() {
		int32 number = FCString::Atoi(*Number);
		return number;
	}

	UFUNCTION(BlueprintCallable)
	void GetPlayersForceList();

	//******************************************************************************************************//

	// Metodo para incializar el request
	TSharedRef<IHttpRequest> InitRequest(FString Subroute);

	// Metodo en donde va propiamente el tipo de request y que llama a RequestWithRoute
	// Si fuera un metodo POST habria que hacer un metodo con las consideraciones pero seguiria
	// canalizando entre SendRequest e InitRequest
	TSharedRef<IHttpRequest> GetRequest(FString Subroute);
	
	// Metodo que envia propiamente el request
	void SendRequest(TSharedRef<IHttpRequest>& Request);

	// Metodo que se encarga de determinar el resultado del request (la respuesta)
	bool ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful);

	//*******************************************************************************************************//

	// Sets default values for this actor's properties
	AMyHttpActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;   	  	

};

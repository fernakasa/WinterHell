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
struct FResponse_PlayerList {
	GENERATED_BODY()
	UPROPERTY() FString name;
	UPROPERTY() FString plays;

	FResponse_PlayerList() {}
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

	EHttpResponseCodes code;

	// cabeza de autorizacion
	// = TEXT("User-Agent"), "X-UnrealEngine-Agent" para agentes unreal
	// = "Content-Type", TEXT("application/json" para archivos JSON
	//FString AuthorizationHeader = TEXT("Authorization");

	//template para la estructura JSON que me va a devolver la BD
	template <typename StructType>
	void GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput);


public:
	// Metodo para testear que el actor c++ tome un parametro del blueprint y lo devuelva (bypass)
	UFUNCTION(BlueprintCallable)
	FString TestHelloWorld(FString PlayerName);

	UFUNCTION(BlueprintCallable)
	bool TestApiConn();



	TSharedRef<IHttpRequest> RequestWithRoute(FString Subroute);
	TSharedRef<IHttpRequest> GetRequest(FString Subroute);
	void Send(TSharedRef<IHttpRequest>& Request);
	bool ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful);




	// Funcion POST request
	UFUNCTION(BlueprintCallable)
	void PostRequest(FString PlayerName);

	// Funcion GET
	UFUNCTION(BlueprintCallable)
	void GetRequest();


	// Funcion que toma codigo de repuesta del request
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnResponseGetReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// Sets default values for this actor's properties
	AMyHttpActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};

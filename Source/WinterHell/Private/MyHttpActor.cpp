#include "WinterHell.h"
#include "MyHttpActor.h"



// Sets default values
AMyHttpActor::AMyHttpActor()
{

	Http = &FHttpModule::Get();
}


void AMyHttpActor::BeginPlay()
{
	Super::BeginPlay();
	
}


// Metodo de prueba para chequear interconexion BP + C++
FString AMyHttpActor::TestHelloWorld(FString PlayerName) {
	FString name = PlayerName;
	return name;
}



// Metodo para hacer un post request
void AMyHttpActor::PostRequest(FString PlayerName) {
	// crear el request usando el modulo FHttpModule
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	// bindear la funcion que se va a disparar cuando recibimos la respuesta del process request
	Request->OnProcessRequestComplete().BindUObject(this, &AMyHttpActor::OnResponseReceived);
	
	// nombre da la url (api) que ya esta almacenada
	//FString apiRoute = ApiBaseUrl + "guardar/Guachiturro";
	FString apiRoute = ApiBaseUrl + "guardar/" + PlayerName;
	Request->SetURL(apiRoute);

	// Tipo de metodo http (mandando data es POST)
	Request->SetVerb("GET");
	/*UE_LOG(LogTemp, Warning, TEXT(apiRoute));*/
	//Consultar por cabeceras!!
	// Le decimos al host que somos un agente Unreal
	//Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");

	// le pasamos el nombre del jugador que es accesible y transferible desde el Blueprint
	//Request->SetContentAsString(PlayerName);

	// Procesamos el pedido
	Request->ProcessRequest();
}

void AMyHttpActor::GetRequest() {
	// crear el request usando el modulo FHttpModule
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	// bindear la funcion que se va a disparar cuando recibimos la respuesta del process request
	Request->OnProcessRequestComplete().BindUObject(this, &AMyHttpActor::OnResponseGetReceived);

	// nombre da la url (api) que ya esta almacenada
	FString apiRoute = ApiBaseUrl + "players";
	Request->SetURL(ApiBaseUrl);

	// Tipo de metodo http (mandando data es POST)
	Request->SetVerb("GET");

	//Consultar por cabeceras!!
	// Le decimos al host que somos un agente Unreal
	//Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");

	// Procesamos el pedido
	Request->ProcessRequest();
}

void AMyHttpActor::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	// Evalua la respuesta

	
	//code = Response->GetResponseCode();
	code = 200;
}

void AMyHttpActor::OnResponseGetReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	// Evalua la respuesta

	code = Response->GetResponseCode();

	// Guarda la estructura JSON  de la respuesta
	FResponse_PlayerList OnResponseGetReceived;
	GetStructFromJsonString<FResponse_PlayerList>(Response, OnResponseGetReceived);

	// Ahora a ver que hacemos con esa estructura



}

int AMyHttpActor::GetCode() {
	return code;
}


template <typename StructType>
void AMyHttpActor::GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput) {
	StructType StructData;
	FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString, &StructOutput, 0, 0);
}


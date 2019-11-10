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
	TestApiConn();
}

//*******************************************************************************************//

TSharedRef<IHttpRequest> AMyHttpActor::InitRequest(FString Subroute) {
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetURL(ApiBaseUrl + Subroute);
	return Request;
}

TSharedRef<IHttpRequest> AMyHttpActor::GetRequest(FString Subroute) {
	TSharedRef<IHttpRequest> Request = InitRequest(Subroute);
	Request->SetVerb("GET");
	return Request;
}

void AMyHttpActor::SendRequest(TSharedRef<IHttpRequest>& Request) {
	Request->ProcessRequest();
}


// Este metodo determina la respuesta
bool AMyHttpActor::ResponseIsValid(FHttpResponsePtr Response, bool Success) {
	if (!Success || !Response.IsValid()) { return false; }
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) { 
		return true; 
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Codigo de error: %d"), Response->GetResponseCode());
		return false;
	}
}

//********************************************************************************************//

// Metodo de prueba para chequear interconexion BP + C++
FString AMyHttpActor::TestHelloWorld(FString PlayerName) {
	FString name = PlayerName;

	return name;
}

void AMyHttpActor::SetRequestSuccess(bool value) {
	requestSuccess = value;
}

bool AMyHttpActor::GetRequestSuccess() {
	return requestSuccess;
}

void AMyHttpActor::TestApiConn() {
	// Llamo a GetRequest pasandole el nombre de mi controlador
	// GetRequest va a llamar a InitRequest pasandole el tipo de pedido (GET)
	// y el nombre del controlador ("test") para que lo añada a la ruta y pueda llegar a el
	TSharedRef<IHttpRequest> Request = GetRequest("test");
	Request->OnProcessRequestComplete().BindUObject(this, &AMyHttpActor::TestResponse);
	SendRequest(Request);
}


void AMyHttpActor::TestResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success) {
	if (!ResponseIsValid(Response, Success)) {
		SetRequestSuccess(false);
		return;
	}
	SetRequestSuccess(true);
}


// Metodo que llama a Request con la subruta '/guardar/nombre_jugador'
// Suma al jugador una jugada (de existir) o lo crea (de no existir)
void AMyHttpActor::InsertPlayer(FString PlayerName) {
	TSharedRef<IHttpRequest> Request = GetRequest("guardar/" + PlayerName);
	Request->OnProcessRequestComplete().BindUObject(this, &AMyHttpActor::TestResponse);
	SendRequest(Request);
}


// Metodo para insertar el nombre del jugador y sumar uno
//void AMyHttpActor::PostRequest(FString PlayerName) {
//
//	// crear el request usando el modulo FHttpModule
//	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
//
//	// bindear la funcion que se va a disparar cuando recibimos la respuesta del process request
//	Request->OnProcessRequestComplete().BindUObject(this, &AMyHttpActor::OnResponseReceived);
//	
//	// nombre da la url (api) que ya esta almacenada
//	//FString apiRoute = ApiBaseUrl + "guardar/Guachiturro";
//	FString apiRoute = ApiBaseUrl + "guardar/" + PlayerName;
//	Request->SetURL(apiRoute);
//
//	// Tipo de metodo http (mandando data es POST)
//	Request->SetVerb("GET");
//	/*UE_LOG(LogTemp, Warning, TEXT(apiRoute));*/
//	//Consultar por cabeceras!!
//	// Le decimos al host que somos un agente Unreal
//	//Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
//
//	// le pasamos el nombre del jugador que es accesible y transferible desde el Blueprint
//	//Request->SetContentAsString(PlayerName);
//
//	// Procesamos el pedido
//	Request->ProcessRequest();
//}
//
//void AMyHttpActor::GetRequest() {
//	// crear el request usando el modulo FHttpModule
//	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
//	// bindear la funcion que se va a disparar cuando recibimos la respuesta del process request
//	Request->OnProcessRequestComplete().BindUObject(this, &AMyHttpActor::OnResponseGetReceived);
//
//	// nombre da la url (api) que ya esta almacenada
//	FString apiRoute = ApiBaseUrl + "players";
//	Request->SetURL(ApiBaseUrl);
//
//	// Tipo de metodo http (mandando data es POST)
//	Request->SetVerb("GET");
//
//	//Consultar por cabeceras!!
//	// Le decimos al host que somos un agente Unreal
//	//Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
//
//	// Procesamos el pedido
//	Request->ProcessRequest();
//}
//
//void AMyHttpActor::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
//	// Evalua la respuesta
//
//	
//	//code = Response->GetResponseCode();
//	//code = 200;
//}
//
//void AMyHttpActor::OnResponseGetReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
//	// Evalua la respuesta
//
//	//code = Response->GetResponseCode();
//
//	// Guarda la estructura JSON  de la respuesta
//	//FResponse_PlayerList OnResponseGetReceived;
//	//GetStructFromJsonString<FResponse_PlayerList>(Response, OnResponseGetReceived);
//
//	// Ahora a ver que hacemos con esa estructura
//
//
//
//}

template <typename StructType>
void AMyHttpActor::GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput) {
	StructType StructData;
	FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString, &StructOutput, 0, 0);
}


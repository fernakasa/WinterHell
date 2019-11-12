#include "JsonUtilities.h"
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
	//SetPlayersNames();
	//SetPlayersPlays();
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

TArray<FString> AMyHttpActor::GetPlayersNames() {
	return PlayersNames;
}

TArray<FString> AMyHttpActor::GetPlayersPlays() {
	return PlayersPlays;
}

//void AMyHttpActor::SetPlayersNames(FString value) {
//	PlayersNames.Add(TEXT(value));
//}
//
//void AMyHttpActor::SetPlayersPlays(FString value) {
//	PlayersPlays.Add(TEXT(value));
//}

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


void AMyHttpActor::GetPlayersList() {	
	TSharedRef<IHttpRequest> Request = GetRequest("players");
	Request->OnProcessRequestComplete().BindUObject(this, &AMyHttpActor::PlayerListResponse);
	SendRequest(Request);
}

void AMyHttpActor::PlayerListResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success) {
	if (!ResponseIsValid(Response, Success)) {
		//UE_LOG(LogTemp, Warning, TEXT("Error"));
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Paso"));

	FResponse_PlayerList ListResponse;
	GetStructFromJsonString<FResponse_PlayerList>(Response, ListResponse);
	
	int32 c = 0;
	if (c == 0) {
		//PlayersNames.AddUnique(ListResponse.nickname);
		PlayersNames.Add(ListResponse.nickname);
		PlayersPlays.Add(ListResponse.cantidadJugadas);
		c++;
	}
		   	

	//UE_LOG(LogTemp, Warning, TEXT("Name is: %s"), *ListResponse.nickname);
	//UE_LOG(LogTemp, Warning, TEXT("Plays is: %s"), *ListResponse.cantidadJugadas);	
		
}


//****************************************************
// Workaround para generar la lista
void AMyHttpActor::GetPlayersForceList() {
	GetNumberOfPlayers();
	int32 numberPlayers = GetNumber();
	for (size_t i = 0; i < numberPlayers; i++)
	{
		FString stringIndex = FString::FromInt(i);
		TSharedRef<IHttpRequest> Request = GetRequest("playersByIndex/" + stringIndex);
		Request->OnProcessRequestComplete().BindUObject(this, &AMyHttpActor::PlayerListResponse);
		SendRequest(Request);
	}

	
}


//********************************
void AMyHttpActor::GetNumberOfPlayers() {
	TSharedRef<IHttpRequest> Request = GetRequest("number");
	Request->OnProcessRequestComplete().BindUObject(this, &AMyHttpActor::GetNumberOfPlayersResponse);
	SendRequest(Request);	
}

void AMyHttpActor::GetNumberOfPlayersResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success) {

	Number = Response->GetContentAsString();	
}

void AMyHttpActor::EmptyLists() {
	PlayersNames.Empty();
	PlayersPlays.Empty();
}


template <typename StructType>
void AMyHttpActor::GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput) {
	StructType StructData;
	FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString, &StructOutput, 0, 0);
}









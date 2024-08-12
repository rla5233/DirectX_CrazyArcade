#include "PreCompile.h"
#include "ServerGameMode.h"
#include "Player.h"
#include <EngineCore/Image.h>
#include <EngineCore/Camera.h>
#include <EngineBase/EngineSerializer.h>
#include <EngineCore/BlurEffect.h>
#include <EngineCore/EngineEditorGUI.h>
#include "CrazyArcadeCore.h"
#include "Packets.h"
#include "ServerTestPlayer.h"
#include "ServerTestOtherPlayer.h"
#include <EngineBase/NetObject.h>
#include "ServerHelper.h"
#include "CrazyArcadeEnum.h"
#include "BombBase.h"
#include "MapBase.h"
#include "ServerManager.h"

AServerGameMode::AServerGameMode()
	:AMainPlayLevel()
{
}

AServerGameMode::~AServerGameMode()
{

}

void AServerGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void AServerGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	UNetObject::AllNetObject;
	int a = 0;

	CheckGame(_DeltaTime);
}

void AServerGameMode::LevelStart(ULevel* _PrevLevel)
{
	Super::LevelStart(_PrevLevel);
	if (Player->GetObjectToken() == -1) {
		Player->SetObjectToken(UCrazyArcadeCore::Net->GetSessionToken() * 1000);
	}
	ConnectionInfo::GetInst().TeamCount();
}


void AServerGameMode::HandlerInit()
{
	UEngineDispatcher& Dis = UCrazyArcadeCore::Net->Dispatcher;
	if (ENetType::Server == UCrazyArcadeCore::NetManager.GetNetType()) {
		Dis.AddHandler<UActorUpdatePacket>([=](std::shared_ptr<UActorUpdatePacket> _Packet)
			{
				if (nullptr == UCrazyArcadeCore::Net)
				{
					MsgBoxAssert("�̰ſֵ���?");
				}
				UCrazyArcadeCore::Net->Send(_Packet);

				GetWorld()->PushFunction([=]()
					{
						// ���� �־�����ϱ�.
						ANetActor* OtherPlayer = UNetObject::GetNetObject<ANetActor>(_Packet->GetObjectToken());
						if (nullptr == OtherPlayer)
						{
							ServerHelper::EnumSpawn(GetWorld(), _Packet->SpawnSelect, _Packet->GetObjectToken());
							OtherPlayer = UNetObject::GetNetObject<ANetActor>(_Packet->GetObjectToken());
						}

						OtherPlayer->PushProtocol(_Packet);
					});


			});

		Dis.AddHandler<USpawnUpdatePacket>([=](std::shared_ptr<USpawnUpdatePacket> _Packet)  //���� ���� �׽�Ʈ��
			{
				GetWorld()->PushFunction([=]()
					{
						ABombBase* Bomb = UNetObject::GetNetObject<ABombBase>(_Packet->GetObjectToken());
						if (Bomb != nullptr) {
							MsgBoxAssert("�̰ŵ���������ȵ�����ȵ�����ȵ�����ȵ�����ȵ�����ȵ�")   // -Test-
						}

						ServerHelper::EnumSpawn(GetWorld(), _Packet->SpawnSelect, _Packet->GetObjectToken(), _Packet->Pos);
						Bomb = UNetObject::GetNetObject<ABombBase>(_Packet->GetObjectToken());
						Bomb->SetObjectToken(_Packet->GetObjectToken());
						Bomb->SetBombPower(_Packet->Power);
						Bomb->PushProtocol(_Packet);
						UCrazyArcadeCore::Net->Send(_Packet);

						FEngineTimeStamp curtime = UEngineTime::GetCurTime();
					});
			});

		Dis.AddHandler<UBlockUpdatePacket>([=](std::shared_ptr<UBlockUpdatePacket> _Packet)  //���� ���� �׽�Ʈ��
			{
				GetWorld()->PushFunction([=]()
					{
						UNetObject* Block = UNetObject::GetNetObject<UNetObject>(_Packet->GetObjectToken());
						if (nullptr == Block) {
							if (_Packet->IsDestroy == true) {  //���� null�ε�, �μ�����ϳ�? Ȥ�� �ٸ��ֵ� �� �μ����� �ֳ�? �� Ȯ���ؼ� �μŶ�
								UCrazyArcadeCore::Net->Send(_Packet);
								return;
							}
							//MsgBoxLogT("���� �˸�", "�������� ������� �ѵ�, �ð������� ��ġ�� �׸��� �� MsgBoxLog�� ���� �� �����");
							_Packet->IsDestroy = true;
							_Packet->SetSessionToken(UCrazyArcadeCore::Net->GetSessionToken());
							UCrazyArcadeCore::Net->Send(_Packet);  //�̹� �μ��� �����̹Ƿ�, PushProtocol ���ص���
							return;
						}
						Block->PushProtocol(_Packet);
						UCrazyArcadeCore::Net->Send(_Packet);
					});
			});
	}

	if (ENetType::Client == UCrazyArcadeCore::NetManager.GetNetType()) {
		Dis.AddHandler<UActorUpdatePacket>([=](std::shared_ptr<UActorUpdatePacket> _Packet)
			{
				if (nullptr == UCrazyArcadeCore::Net)
				{
					return;
					MsgBoxAssert("�̰ſֵ���?");

				}
				GetWorld()->PushFunction([=]()
					{
						// ���� �־�����ϱ�.
						ANetActor* OtherPlayer = UNetObject::GetNetObject<ANetActor>(_Packet->GetObjectToken());
						if (nullptr == OtherPlayer)
						{
							ServerHelper::EnumSpawn(GetWorld(), _Packet->SpawnSelect, _Packet->GetObjectToken());
							OtherPlayer = UNetObject::GetNetObject<ANetActor>(_Packet->GetObjectToken());
						}

						OtherPlayer->PushProtocol(_Packet);
					});
			});

		Dis.AddHandler<USpawnUpdatePacket>([=](std::shared_ptr<USpawnUpdatePacket> _Packet)  //���� ���� �׽�Ʈ��
			{
				GetWorld()->PushFunction([=]()
					{
						ABombBase* Bomb = UNetObject::GetNetObject<ABombBase>(_Packet->GetObjectToken());
						if (Bomb != nullptr) {
							MsgBoxAssert("�̰ŵ���������ȵ�����ȵ�����ȵ�����ȵ�����ȵ�����ȵ�")   // -Test-
						}

						ServerHelper::EnumSpawn(GetWorld(), _Packet->SpawnSelect, _Packet->GetObjectToken(), _Packet->Pos);
						Bomb = UNetObject::GetNetObject<ABombBase>(_Packet->GetObjectToken());
						Bomb->SetObjectToken(_Packet->GetObjectToken());
						Bomb->SetBombPower(_Packet->Power);
						Bomb->PushProtocol(_Packet);

						FEngineTimeStamp CurStamp = UEngineTime::GetCurTime();
						float PacketDiff = CurStamp.TimeToFloat() - _Packet->SpawnTime;
					});
			});
		Dis.AddHandler<UBlockUpdatePacket>([=](std::shared_ptr<UBlockUpdatePacket> _Packet)  //���� ���� �׽�Ʈ��
			{
				GetWorld()->PushFunction([=]()
					{
						UNetObject* Block = UNetObject::GetNetObject<UNetObject>(_Packet->GetObjectToken());
						if (nullptr == Block) {
							if (_Packet->IsDestroy == true) { //���� �μ����µ�, �μ������ ���³�? �� Ŭ��ϱ� �׳� �����ؾ���
								return;
							}
							//MsgBoxLogT("���� �˸�", "�������� ������� �ѵ�, �ð������� ��ġ�� �׸��� �� MsgBoxLog�� ���� �� �����");
							_Packet->IsDestroy = true;
							_Packet->SetSessionToken(UCrazyArcadeCore::Net->GetSessionToken());
							UCrazyArcadeCore::Net->Send(_Packet);  //�̹� �μ��� �����̹Ƿ�, PushProtocol ���ص���
							return;
						}
						Block->PushProtocol(_Packet);
					});
			});
	}
}

void AServerGameMode::LevelEnd(ULevel* _NextLevel)
{
	Super::LevelEnd(_NextLevel);
	std::map<int, ConnectUserInfo>& Temp = ConnectionInfo::GetInst().GetUserInfos();
	for (std::pair<const int, ConnectUserInfo>& Iterator : Temp)
	{
		Iterator.second.SetIsDead(false);
	}
	ConnectionInfo::GetInst().SetWins(ECharacterColor::None);
}

void AServerGameMode::CheckGame(float _DeltaTime)
{
	bool WinResult = false;
	ECharacterColor WinTeam = ConnectionInfo::GetInst().GetWins();
	switch (WinTeam)
	{
	case ECharacterColor::Red:
		WinResult = true;
		break;
	case ECharacterColor::Blue:
		WinResult = true;
		break;
	case ECharacterColor::Green:
		WinResult = true;
		break;
	case ECharacterColor::None:
		WinResult = false;
		break;
	}
	if (true == WinResult)
	{
		CurWinTime += _DeltaTime;
	}
	if (CurWinTime >= WinTime && ENetType::Server == UCrazyArcadeCore::NetManager.GetNetType())
	{
		CurWinTime = 0.0f;
		std::shared_ptr<UChangeLevelPacket> Packet = std::make_shared<UChangeLevelPacket>();
		Packet->LevelName = "LobbyLevel";
		UCrazyArcadeCore::NetManager.Send(Packet);
		GEngine->ChangeLevel("LobbyLevel");
		return;
	}
}

#include"PreCompile.h"
#include "InGameUI.h"
#include "Player.h"
#include "CrazyArcadeCore.h"
#include "ConnectionInfo.h"
#include "ServerManager.h"
#include "Packets.h"
#include "MainPlayLevel.h"
#include "ServerTestPlayer.h"

AInGameUI::AInGameUI()
{
}

AInGameUI::~AInGameUI()
{
}

void AInGameUI::BeginPlay()
{
	WinSound = UEngineSound::SoundPlay("Win_KCW.mp3");
	WinSound.Off();
	Losesound = UEngineSound::SoundPlay("Lose_Effect.wav");
	Losesound.Off();

	// �̹��� Ŀ��
	{
		UEngineSprite::CreateCutting("Play_ExitButtonHover.png", 1, 2);

		//����
		UEngineSprite::CreateCutting("Play_Portrait_Bazzi_Lose_R.png", 1, 4);
		UEngineSprite::CreateCutting("Play_Portrait_Bazzi_Normal_R.png", 1, 2);
		UEngineSprite::CreateCutting("Play_Portrait_Bazzi_Lose_B.png", 1, 4);
		UEngineSprite::CreateCutting("Play_Portrait_Bazzi_Normal_B.png", 1, 2);

		//�ٿ�
		UEngineSprite::CreateCutting("Play_Portrait_Dao_Normal_R.png", 1, 2);
		UEngineSprite::CreateCutting("Play_Portrait_Dao_Lose_R.png", 1, 4);
		UEngineSprite::CreateCutting("Play_Portrait_Dao_Lose_B.png", 1, 4);
		UEngineSprite::CreateCutting("Play_Portrait_Dao_Normal_B.png", 1, 2);

		//������
		UEngineSprite::CreateCutting("Play_Portrait_Marid_Lose_B.png", 1, 4);
		UEngineSprite::CreateCutting("Play_Portrait_Marid_Lose_R.png", 1, 4);
		UEngineSprite::CreateCutting("Play_Portrait_Marid_Normal_B.png", 1, 2);
		UEngineSprite::CreateCutting("Play_Portrait_Marid_Normal_R.png", 1, 2);
	}
	
	Super::BeginPlay();

	//��ư 
	CancelBtn = CreateWidget<UImage>(GetWorld(), "CancelButton");
	CancelBtn->CreateAnimation("CancelButtonUnHoverAni", "Play_ExitButtonUnHover.png", 0.1f, false, 0, 0);
	CancelBtn->CreateAnimation("CancelButtonHoverAni", "Play_ExitButtonHover.png", 0.1f, false, 0, 1);
	CancelBtn->CreateAnimation("CancelButtonIsDownAni", "Play_ExitButtonIsDown.png", 0.1f, false, 0, 0);

	CancelBtn->SetAutoSize(1.0f, true);
	CancelBtn->ChangeAnimation("CancelButtonUnHoverAni");
	CancelBtn->SetWidgetLocation({ 316.0f, -276.0f });
	CancelBtn->AddToViewPort(3);

	CancelBtn_InActive = CreateWidget<UImage>(GetWorld(), "CancelBtn_InActive");
	CancelBtn_InActive->SetSprite("Play_ExitButton_InActive.png");
	CancelBtn_InActive->SetMulColor({ 1.0f, 1.0f, 1.0f, 0.5f });
	CancelBtn_InActive->AddToViewPort(3);
	CancelBtn_InActive->SetAutoSize(1.0f, true);
	CancelBtn_InActive->SetWidgetLocation({ 316.0f, -276.0f });
	

	//���â

	ResultBackGround = CreateWidget<UImage>(GetWorld(), "UIResult");
	ResultBackGround->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
	ResultBackGround->AddWidgetLocation(FVector{ -20.0f,100.0f });
	ResultBackGround->AddToViewPort(3);
	ResultBackGround->SetAutoSize(1.0f, true);
	

	
	
	//�ٴ�
	NeedleRender2 = CreateWidget<UImage>(GetWorld(), "NeedleUI2");
	NeedleRender2->SetSprite("spr_item_needle.png");
	NeedleRender2->SetMulColor({ 1.0f,1.0f,1.0f,2.0f });
	NeedleRender2->SetScale({ 40,40 });
	NeedleRender2->AddWidgetLocation({ 294,-210 });
	NeedleRender2->AddToViewPort(4);



	NeedleNum = CreateWidget<UImage>(GetWorld(), "NeedleNum0");
	NeedleNum->SetSprite("0.png");
	NeedleNum->SetAutoSize(1.0f, true);
	NeedleNum->AddWidgetLocation({ 352,-210 });
	NeedleNum->AddToViewPort(4);

	NeedleNum2 = CreateWidget<UImage>(GetWorld(), "NeedleNum0");
	NeedleNum2->SetAutoSize(1.0f, true);
	NeedleNum2->AddWidgetLocation({ 364,-210 });
	NeedleNum2->AddToViewPort(4);

	NeedleX = CreateWidget<UImage>(GetWorld(), "XUI");
	NeedleX->SetSprite("X.png");
	NeedleX->SetAutoSize(1.0f,true);
	NeedleX->AddWidgetLocation({ 340,-210 });
	NeedleX->AddToViewPort(4);


	CancelBtn->SetUnHover([=] {
		CancelBtn->ChangeAnimation("CancelButtonUnHoverAni");
		});
	CancelBtn->SetHover([=] {
		if (ENetType::Server == UCrazyArcadeCore::NetManager.GetNetType())
		{
			CancelBtn->ChangeAnimation("CancelButtonHoverAni");
		}
		});
	CancelBtn->SetDown([=] {
		if (ENetType::Server == UCrazyArcadeCore::NetManager.GetNetType())
		{
			CancelBtn->ChangeAnimation("CancelButtonIsDownAni");
		}
		});
	CancelBtn->SetPress([=] {

		});
	CancelBtn->SetUp([=] {
		if (ENetType::Server == UCrazyArcadeCore::NetManager.GetNetType())
		{
			ChangeFadeOut(true);
		}
		});

	// Fade
	{
		Fade = CreateWidget<UImage>(GetWorld(), "Fade");
		Fade->SetSprite("FadeBlack.png");
		Fade->AddToViewPort(10);
		Fade->SetAutoSize(1.0f, true);
		Fade->SetWidgetLocation({ 0.0f, 0.0f });
		Fade->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
	}

	//���� ���� �ϸ� ���� ���� �����ͼ� ǥ�����ֱ� 
	for (int i = 0; i < 8; i++)
	{

		UImage* Render = CreateWidget<UImage>(GetWorld(), "PlayerRender" + i);

		Render->CreateAnimation("BazziRedNormal", "Play_Portrait_Bazzi_Normal_R.png", 0.8f, true, 0, 1);
		Render->CreateAnimation("BazziRedDead", "Play_Portrait_Bazzi_Lose_R.png", 0.4f, true, 0, 3);
		Render->CreateAnimation("BazziBlueNormal", "Play_Portrait_Bazzi_Normal_B.png", 0.8f, true, 0, 1);
		Render->CreateAnimation("BazziBlueDead", "Play_Portrait_Bazzi_Lose_B.png", 0.4f, true, 0, 3);

		Render->CreateAnimation("DaoRedNormal", "Play_Portrait_Dao_Normal_R.png", 0.8f, true, 0, 1);
		Render->CreateAnimation("DaoRedDead", "Play_Portrait_Dao_Lose_R.png", 0.4f, true, 0, 3);
		Render->CreateAnimation("DaoBlueNormal", "Play_Portrait_Dao_Normal_B.png", 0.8f, true, 0, 1);
		Render->CreateAnimation("DaoBlueDead", "Play_Portrait_Dao_Lose_B.png", 0.4f, true, 0, 3);

		Render->CreateAnimation("MaridRedNormal", "Play_Portrait_Marid_Normal_R.png", 0.8f, true, 0, 1);
		Render->CreateAnimation("MaridRedDead", "Play_Portrait_Marid_Lose_R.png", 0.4f, true, 0, 3);
		Render->CreateAnimation("MaridBlueNormal", "Play_Portrait_Marid_Normal_B.png", 0.8f, true, 0, 1);
		Render->CreateAnimation("MaridBlueDead", "Play_Portrait_Marid_Lose_B.png", 0.4f, true, 0, 3);

		Render->SetAutoSize(1.0f, true);
		Render->AddToViewPort(3);
		PlayerUI.push_back(Render);

		UTextWidget* Name = CreateWidget<UTextWidget>(GetWorld(), "PlayerNameUI" + i);
		Name->SetFont("����");
		Name->SetScale(12.0f);
		Name->SetColor(Color8Bit::White);
		Name->AddToViewPort(3);
		Name->SetFlag(FW1_LEFT);
		Name->SetPosition({ 0 * (i + 20),0 });

		PlayerNameUI.push_back(Name);

		PlayerState State;
		State.Order = i;
		State.PlayerName = "";
		State.PlayerColor = ECharacterColor::Red;
		State.PlayerType = ECharacterType::Random;
		State.IsDead = false;
		PlayerInfo.push_back(State);
	}

	//InitPlayerInfo();
	//DataToRender();

	ResultBackGround->SetActive(false);
	
	std::vector<UImage*> PlayerUI; //�÷��̾�
	std::vector<UTextWidget*> PlayerNameUI;
	std::vector<PlayerState> PlayerInfo;
}

void AInGameUI::LevelStart(ULevel* _PrevLevel)
{
	Super::LevelStart(_PrevLevel);

	InitPlayerInfo();
	DataToRender();
	PlayerLevelPtr = dynamic_cast<AMainPlayLevel*>(GetWorld()->GetGameMode().get());
	PlayerPtr = PlayerLevelPtr->GetPlayer().get();
	Needles = PlayerPtr->GetNeedleCount();
	
	for (int i = 0; i < PlayerInfo.size(); i++)
	{
		PlayerInfo[i].IsChange = false;
		std::string AnimName = StateToAnimName(PlayerInfo[i].PlayerType, PlayerInfo[i].PlayerColor, false);
		PlayerUI[i]->ChangeAnimation(AnimName);
	}

	// CancelBtn�� ������ ���� �� ����
	if (ENetType::Server == UCrazyArcadeCore::NetManager.GetNetType())
	{
		CancelBtn_InActive->SetActive(false);
	}

	// Fade
	IsFadeIn = true;
	FadeAlpha = 1.0f;
}

void AInGameUI::LevelEnd(ULevel* _NextLevel)
{
	Super::LevelEnd(_NextLevel);
	ResultBackGround->SetActive(false);
}

void AInGameUI::Tick(float _DeltaTIme)
{
	Super::Tick(_DeltaTIme);

	// Fade & ChangeLevel
	{
		if (ENetType::Server != UCrazyArcadeCore::NetManager.GetNetType())
		{
			IsFadeOut = ConnectionInfo::GetInst().GetIsFadeOut();
		}

		if (IsFadeIn == true)
		{
			FadeIn(_DeltaTIme);
		}

		if (IsFadeOut == true)
		{
			FadeOut(_DeltaTIme);
		}
	}

	//�׾����� ��Ҵ��� Ȯ���ϴ� ���� �ڵ�(������)

	if (this->IsActive() == true)
	{
		PlayerUpdate();
	}

	DeadCheck(); //�׾����� ���¸� �ٲ��ش� 


	for (int i = 0; i < PlayerInfo.size(); i++)
	{
		
		if (PlayerInfo[i].IsDead == true && PlayerInfo[i].IsChange == false)
		{
			std::string AnimName = StateToAnimName(PlayerInfo[i].PlayerType, PlayerInfo[i].PlayerColor, PlayerInfo[i].IsDead);
			PlayerUI[i]->ChangeAnimation(AnimName);
			PlayerInfo[i].IsChange = true; 
		}	//
	}
	//�ٴ� ���� �������� 
	NeedleCheck();
	
	
	ResultCheck();
}



void AInGameUI::PlayerUpdate()
{
	std::map<int, ConnectUserInfo> UserInfos = ConnectionInfo::GetInst().GetUserInfos(); //���� ��������

	for (int i = 0; i < 8; ++i)
	{
		if (false == UserInfos.contains(i))
		{
			// ������ ������ �Ǿ��ִ��� �ȵǾ� �ִ��� Ȯ��
			PlayerUI[i]->SetActive(false);
			PlayerNameUI[i]->SetActive(false);
			continue;
		}
		else
		{
			PlayerUI[i]->SetActive(true);
			PlayerNameUI[i]->SetActive(true);
		}
	}




	


	
}



void AInGameUI::InitPlayerInfo()
{
	//
	std::map<int, ConnectUserInfo> UserInfos = ConnectionInfo::GetInst().GetUserInfos();

	//������ �̸� ��������
	for (std::pair<int, ConnectUserInfo> Iterator : UserInfos)
	{ //���ͷ����ͷ� ������ 
		PlayerInfo[Iterator.first].Order = Iterator.first;
		//PlayerInfo[Iterator.first].IsDead = UserDeadCheck[Iterator.first];
		//�ӽ� �۾��� �ڵ� 	

		PlayerInfo[Iterator.first].PlayerName = Iterator.second.MyName;
		PlayerInfo[Iterator.first].PlayerType = Iterator.second.GetMyCharacterType();
		PlayerInfo[Iterator.first].PlayerColor = Iterator.second.GetMyColorType();
		

	}
}


void AInGameUI::DataToRender()
{


	//PlayerInfo.size()
	for (int i = 0; i < PlayerInfo.size(); i++)
	{
		PlayerNameUI[i]->SetPosition({ 305, 179 - (i * 43) });
		PlayerNameUI[i]->SetText(PlayerInfo[i].PlayerName);
		PlayerUI[i]->SetPosition({ 281, 181 - (i * 43) });
		PlayerStateCheck();
	}



	//���� 

	//ChangeAnmation()

}

std::string AInGameUI::StateToAnimName(ECharacterType _Type, ECharacterColor _Color, bool _IsDead)
{
	return TypeToName(_Type) + ColorToName(_Color) + (_IsDead ? "Dead" : "Normal");

	//�ִϸ��̼� �̸��� ����� 
	//return TypeToName[_Type] + ColorToName[_Color] + (_IsDead ? "Dead" : "Normal");
}


void AInGameUI::PlayerStateCheck()
{
	//���� ������ ���ü��� �������ְ� 
	//ó���� �����Ҷ� ����Ʈ�� ������ ���ֱ� ������ ���� ���ٰ� �Ѵ� ����� �߰��Ͽ���. 
	std::map<int, ConnectUserInfo> UserInfos = ConnectionInfo::GetInst().GetUserInfos();

	int UserCount = static_cast<int>(UserInfos.size());

	for (int i = 0; i < UserCount; ++i)
	{
		
		PlayerUI[i]->SetActive(true);

		std::string AnimName = StateToAnimName(PlayerInfo[i].PlayerType, PlayerInfo[i].PlayerColor, PlayerInfo[i].IsDead);
		PlayerUI[i]->ChangeAnimation(AnimName);
	}

	for (int i = UserCount; i < 8; ++i)
	{
		PlayerUI[i]->SetActive(false);
	}

	



}

std::string AInGameUI::TypeToName(ECharacterType _Type)
{

	switch (_Type)
	{
	case ECharacterType::Bazzi:
		return "Bazzi";

	case ECharacterType::Dao:
		return "Dao";

	case ECharacterType::Marid:
		return "Marid";

	default:
		return "Dao";
		break;
	}
}

std::string AInGameUI::ColorToName(ECharacterColor _Color)
{
	switch (_Color)
	{
	case ECharacterColor::Red:
		return "Red";

	case ECharacterColor::Blue:
		return "Blue";
	default:
		return "Red";
		break;
	}
}

void AInGameUI::DeadCheck()
{
	std::map<int, ConnectUserInfo> Info = ConnectionInfo::GetInst().GetUserInfos();
	
	for (std::pair<int, ConnectUserInfo> Iterator : Info)
	{
		PlayerInfo[Iterator.first].IsDead = Iterator.second.GetIsDead();

	}
}

void AInGameUI::FadeIn(float _DeltaTime)
{
	if (FadeAlpha <= 0.0f)
	{
		IsFadeIn = false;
		return;
	}

	FadeAlpha -= _DeltaTime * 3.0f;
	Fade->SetMulColor(float4(1.0f, 1.0f, 1.0f, FadeAlpha));
}

void AInGameUI::FadeOut(float _DeltaTime)
{
	if (FadeAlpha >= 1.0f)
	{
		if (ENetType::Server == UCrazyArcadeCore::NetManager.GetNetType())
		{
			ChangeFadeOut(false);
			GameEnd();
		}
		return;
	}

	FadeAlpha += _DeltaTime * 3.0f;
	Fade->SetMulColor(float4(1.0f, 1.0f, 1.0f, FadeAlpha));
}

void AInGameUI::ChangeFadeOut(bool _IsFadeOut)
{
	// PlayerInfo
	IsFadeOut = _IsFadeOut;
	ConnectionInfo::GetInst().SetIsFadeOut(_IsFadeOut);

	// ��Ŷ ������
	{
		std::shared_ptr<UFadeOutUpdatePacket> Packet = std::make_shared<UFadeOutUpdatePacket>();
		Packet->IsFadeOut = _IsFadeOut;
		UCrazyArcadeCore::NetManager.Send(Packet);
	}
}

void AInGameUI::GameEnd()
{
	if (ENetType::Server == UCrazyArcadeCore::NetManager.GetNetType())
	{
		std::shared_ptr<UChangeLevelPacket> LevelChangePacket = std::make_shared<UChangeLevelPacket>();
		LevelChangePacket->LevelName = "LobbyLevel";
		UCrazyArcadeCore::Net->Send(LevelChangePacket);

		GEngine->ChangeLevel("LobbyLevel");
	}
}

void AInGameUI::NeedleCheck()
{
	
	Needles = PlayerPtr->GetNeedleCount();
	std::string TextNum = std::to_string(Needles);
	NeedleNum2->SetSprite(TextNum + ".png");


	
	
}


void AInGameUI::ResultCheck()
{
	ECharacterColor WinResult = ConnectionInfo::GetInst().GetWins();
	ECharacterColor MyColor = ConnectionInfo::GetInst().GetCharacterColor();

	if (WinResult != ECharacterColor::None)
	{
		if (WinResult == ECharacterColor::Green)
		{
			//ResultBackGround->SetSprite("Lose.png");
			//Losesound.On();
			ResultBackGround->SetActive(true);
		}
		else
		{
			if (WinResult == MyColor)
			{
				ResultBackGround->SetSprite("Win.png");
				WinSound.On();
				ResultBackGround->SetActive(true);
				
			}
			else
			{
			
				ResultBackGround->SetSprite("Lose.png");
				Losesound.On();
				ResultBackGround->SetActive(true);
				
			}
		}
	}	



}



// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h" //Important, 추천 분해도 해보셈.
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Weapons/WeaponBase.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABasePlayer::ABasePlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
		FRotator(0, -90.f, 0)
	);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	bReplicates = true;

}

// Called when the game starts or when spawned
void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

// Called every frame
void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Tick1 %f"), ElapsedFireTime);

}

// Called to bind functionality to input
void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (UIC)
	{
		UIC->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		UIC->BindAction(IA_Jump, ETriggerEvent::Canceled, this, &ACharacter::StopJumping);

		UIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABasePlayer::Move);

		UIC->BindAction(IA_MouseLook, ETriggerEvent::Triggered, this, &ABasePlayer::Look);

		UIC->BindAction(IA_Jog, ETriggerEvent::Triggered, this, &ABasePlayer::Jog);
		UIC->BindAction(IA_Jog, ETriggerEvent::Completed, this, &ABasePlayer::Jog);

		UIC->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &ABasePlayer::FireStart);
		UIC->BindAction(IA_Fire, ETriggerEvent::Completed, this, &ABasePlayer::FireStop);
		UIC->BindAction(IA_Fire, ETriggerEvent::Canceled, this, &ABasePlayer::FireStop);

		UIC->BindAction(IA_ChangeWeapon, ETriggerEvent::Triggered, this, &ABasePlayer::ChangeWeapon);

		UIC->BindAction(IA_Lean, ETriggerEvent::Triggered, this, &ABasePlayer::Lean);
		UIC->BindAction(IA_Lean, ETriggerEvent::Completed, this, &ABasePlayer::Lean);


		UIC->BindAction(IA_Zoom, ETriggerEvent::Triggered, this, &ABasePlayer::Zoom);
		UIC->BindAction(IA_Zoom, ETriggerEvent::Completed, this, &ABasePlayer::Zoom);
	}

}

void ABasePlayer::Jog(const FInputActionValue& Value)
{

	if (Value.Get<bool>() != bIsLog)
	{
		bIsLog = Value.Get<bool>();

		//내꺼 속력 설정
		C2S_Jog_Implementation(bIsLog);

		//서버에서 속력 설정
		C2S_Jog(bIsLog);
	}
}

void ABasePlayer::C2S_Jog_Implementation(bool Value)
{
	bIsLog = Value;

	if (bIsLog)
	{
		UE_LOG(LogTemp, Warning, TEXT("C2S_Jog_Implementation %f"), JogSpeed);

		GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("C2S_Jog_Implementation %f"), WalkSpeed);

		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void ABasePlayer::Shoot()
{
	//서버 에서만 실행 되게 함.
	if (!HasAuthority() ||
		!bFire  ||
		!IsValid(CurrentWeapon) ||
		CurrentWeapon->WeaponType == EWeaponType::Unarmed ||
		CurrentHP <= 0 ||
		FireRate <= 0.f ||
		!IsValid(Camera) )
	{
		return;
	}


	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (GetBaseAimRotation().Vector() * 999999.9f);

	FireTrace(Start, End);


	//FRotator CurrentRotator = GetControlRotation();
	//float RandomAddPitch = FMath::RandRange(0.5f, 1.5f);
	//CurrentRotator.Pitch += RandomAddPitch;
	//GetController()->SetControlRotation(CurrentRotator);

	//리플렉션 
	//EWeaponType::Unarmed -> "Unarmed"
	//Enum to String(Unarmed, Pistol)
	//S2A
	//const UEnum* EnumPtr = StaticEnum<EWeaponType>();
	//if (EnumPtr)
	//{
	//	FString SectionName = EnumPtr->GetNameStringByValue((int32)CurrentWeapon->WeaponType);
	//	PlayAnimMontage(CurrentWeapon->FireMontage, 1.0f, FName(SectionName));
	//}

	CurrentWeapon->Fire();

}

void ABasePlayer::FireTrace(FVector Start, FVector End)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> IgnoreActors;
	FHitResult OutHit;

	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	if (UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		Start,
		End,
		ObjectTypes,
		true,
		IgnoreActors,
		EDrawDebugTrace::ForDuration,
		OutHit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		3.0f
	))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor : %s"), *OutHit.GetActor()->GetName());

		UE_LOG(LogTemp, Warning, TEXT("Hit BoneName : %s"), *OutHit.BoneName.ToString());

		UGameplayStatics::ApplyPointDamage(
			OutHit.GetActor(),
			50.0f,
			-OutHit.ImpactNormal,
			OutHit,
			GetController(),
			this,
			nullptr
		);

	}
}

void ABasePlayer::C2S_Fire_Implementation(bool Value)
{
	if (!Value) //Stop
	{
		StopTrigger();
		return;
	}

	if ( !IsValid(CurrentWeapon) ||
		CurrentWeapon->WeaponType == EWeaponType::Unarmed ||
		CurrentHP <= 0 ||
		FireRate <= 0.f)
	{
		return;
	}

	bFire = true;

	Shoot();

	if (bFire)
	{
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ABasePlayer::Shoot, FireRate, true);
	}
}

void ABasePlayer::StopTrigger()
{
	bFire = false;

	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}



void ABasePlayer::Move(const FInputActionValue& Value)
{
	FVector2D Direction = Value.Get<FVector2D>();

	AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)) * Direction.Y);
	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)) * Direction.X);

}

void ABasePlayer::Look(const FInputActionValue& Value)
{
	FVector2D Rotation = Value.Get<FVector2D>();





	AddControllerYawInput(Rotation.X);
	AddControllerPitchInput(Rotation.Y);
	//AddControllerRollInput();

}

void ABasePlayer::ChangeWeapon(const FInputActionValue& Value)
{
	float Delta = Value.Get<float>();

	int MaxWeaponCount = HaveWeapons.Num();
	int CurrentUsedWeaponIndex = -1;
	if (MaxWeaponCount <= 1)
	{
		return;
	}

	//현재 사용하는 무기 번호 가져오기
	for (int i = 0; i < MaxWeaponCount; ++i)
	{
		//무기 이름 같냐?
		if (CurrentWeapon.GetClass() == HaveWeapons[i].GetClass())
		{
			CurrentUsedWeaponIndex = i;
		}
	}

	int64 NewUsedWeapon = CurrentUsedWeaponIndex + Delta;

	if (NewUsedWeapon < 0)
	{
		NewUsedWeapon = MaxWeaponCount - 1;
	}
	else if (NewUsedWeapon >= MaxWeaponCount)
	{
		NewUsedWeapon = 0;
	}
	//NewUsedWeapon = FMath::Clamp(NewUsedWeapon, 0, MaxWeaponCount - 1);

	CurrentWeapon->SetActorHiddenInGame(true);

	CurrentWeapon = HaveWeapons[NewUsedWeapon];

	CurrentWeapon->SetActorHiddenInGame(false);
}

void ABasePlayer::Zoom(const FInputActionValue& Value)
{
	if (CurrentWeapon && CurrentWeapon->WeaponType != EWeaponType::Unarmed)
	{
		bIsZoom = Value.Get<bool>();
	}
	else
	{
		bIsZoom = false;
	}
}

void ABasePlayer::Lean(const FInputActionValue& Value)
{
	float Direction = Value.Get<float>();

	TargetLeanAngle = 30.0f * Direction;
}

FRotator ABasePlayer::GetAimRotation() const
{
	const FVector AimWS = GetBaseAimRotation().Vector();
	const FVector AimLS = ActorToWorld().InverseTransformVectorNoScale(AimWS);
	const FRotator AimRotationLS = AimLS.Rotation();

	return AimRotationLS;
}

void ABasePlayer::AttachWeapon(TSubclassOf<class AWeaponBase> WeaponTemplate)
{
	for (auto Weapon : HaveWeapons)
	{
		//클래스 이름 똑같냐?
		if (Weapon->GetClass() == WeaponTemplate)
		{
			return;
		}
	}

	//같은 총 집으면 교체 안 먹기
	AWeaponBase* SpawnWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponTemplate, FTransform());
	HaveWeapons.Add(SpawnWeapon);

	SpawnWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("HandGrip_R"));

	SpawnWeapon->SetOwner(this);

	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
	}

	CurrentWeapon = SpawnWeapon;

}

void ABasePlayer::FireStart(const FInputActionValue& Value)
{
	if (CurrentWeapon && CurrentWeapon->WeaponType != EWeaponType::Unarmed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire %d"), Value.Get<bool>());
		bFire = true;
		C2S_Fire(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire"));
		C2S_AttackCombo();
	}
}

void ABasePlayer::FireStop(const FInputActionValue& Value)
{
	C2S_Fire(false);
}




void ABasePlayer::ChangeBigHeadMode()
{
	bIsBigHeadMode = ~bIsBigHeadMode;
}

void ABasePlayer::PlayMontageMeleeAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		FString SectionName = FString::Printf(TEXT("Attack0%d"), ComboCount);

		float MontageLength = PlayAnimMontage(MeleeAttackMontage, 1.0f, FName(SectionName));
		if (MontageLength > 0)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted) {
				if (!bInterrupted)
				{
					ComboCount = 0;
					PlayingComboIndex = 0;
					bIsMeleeAttacking = false;
				}
				});

			AnimInstance->Montage_SetEndDelegate(EndDelegate);
		}
	}
}

void ABasePlayer::AttackCombo()
{
	if (!bIsMeleeAttacking) //처음 공격
	{
		ComboCount++;
		PlayMontageMeleeAttack();
		bIsMeleeAttacking = true;
		PlayingComboIndex = ComboCount; // 1 = 1
	}
	else if (bIsMeleeAttacking && PlayingComboIndex == ComboCount)
	{
		ComboCount++;
	}

}

void ABasePlayer::CheckCombo()
{
	//anim notify에서 호출
	if (PlayingComboIndex != ComboCount)
	{
		PlayMontageMeleeAttack();
		PlayingComboIndex = ComboCount;
	}
}

void ABasePlayer::CheckHit()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> IgnoreActors;
	TArray<AActor*> OutActors;
	IgnoreActors.Add(this);

	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));



	bool Result = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation() + (GetActorForwardVector() * 150.0f),
		80.0f,
		ObjectTypes,
		ABasePlayer::StaticClass(),
		IgnoreActors,
		OutActors
	);

	if (Result)
	{
		for (auto Actor : OutActors)
		{
			ABasePlayer* DamagedPlayer = Cast<ABasePlayer>(Actor);
			if (DamagedPlayer)
			{
				UGameplayStatics::ApplyDamage(
					DamagedPlayer,
					50.0f,
					GetController(),
					this,
					nullptr
				);
			}
		}
	}
}

float ABasePlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHP -= DamageAmount;
	if (CurrentHP > 0)
	{
		GetCharacterMovement()->AddImpulse((GetActorForwardVector() + GetActorUpVector()) * GetMesh()->GetMass() * 7.0f, true);
		PlayAnimMontage(HitRectionMontage);
	}
	else
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->AddImpulse((GetActorForwardVector() + GetActorUpVector()) * GetMesh()->GetMass() * 400.0f);
	}

	return 0.0f;
}

void ABasePlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasePlayer, bIsLog);
	DOREPLIFETIME(ABasePlayer, bFire);
}


void ABasePlayer::S2A_AttackCombo_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("S2A_AttackCombo_Implementation"));
	AttackCombo();
}

void ABasePlayer::C2S_AttackCombo_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("C2S_AttackCombo_Implementation"));
	S2A_AttackCombo();
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "Chat/SocialReadOnlyChatChannel.h"
#include "User/SocialUser.h"
#include "SocialToolkit.h"
#include "Chat/SocialChatManager.h"

void USocialReadOnlyChatChannel::Initialize(USocialUser* InSocialUser, const FChatRoomId& InChannelId, ESocialChannelType InSourceChannelType)
{
	SetChannelType(ESocialChannelType::General);
	SetChannelDisplayName(NSLOCTEXT("AllChatChannelNS","AllChatChannelKey","All"));
}

bool USocialReadOnlyChatChannel::SendMessage(const FString& InMessage)
{
	return false;
}
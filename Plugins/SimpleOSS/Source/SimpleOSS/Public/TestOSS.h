// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <OSSThirdParty/include/alibabacloud/oss/client/RateLimiter.h>
#include <OSSThirdParty/include/alibabacloud/oss/client/RetryStrategy.h>
using namespace AlibabaCloud::OSS;
/**
 * 
 */
class SIMPLEOSS_API TestOSS : public RateLimiter
{
public:
	TestOSS()
		:M_Rate(0)
	{

	}

	~TestOSS();

	virtual void setRate(int rate) { M_Rate = rate; };
	virtual int Rate() const { return M_Rate; };
private:
	int32 M_Rate;
};

//class UserRetryStrategy : public RetryStrategy
//{
//public:
//
//	/* maxRetries表示最大重试次数，scaleFactor为重试等待时间的尺度因子。*/
//	UserRetryStrategy(long maxRetries = 3, long scaleFactor = 300) :
//		m_scaleFactor(scaleFactor), m_maxRetries(maxRetries)
//	{}
//
//	/* 您可以自定义shouldRetry函数，该函数用于判断是否进行重试。*/
//	bool shouldRetry(const Error & error, long attemptedRetries) const;
//
//	/* 您可以自定义calcDelayTimeMs函数，该函数用于计算重试的延迟等待时间。*/
//	long calcDelayTimeMs(const Error & error, long attemptedRetries) const;
//
//private:
//	long m_scaleFactor;
//	long m_maxRetries;
//};

namespace OSSTest
{
	SIMPLEOSS_API int32 MainOSS();
}
/****************************************************************************
Copyright (c) 2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCComAudio.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"

NS_CC_EXT_BEGIN

IMPLEMENT_CLASS_COMPONENT_INFO(CCComAudio)
CCComAudio::CCComAudio(void)
: m_strFilePath("")
, m_bLoop(false)
{
    m_strName = "CCComAudio";
}

CCComAudio::~CCComAudio(void)
{
    
}

bool CCComAudio::init()
{
    return true;
}

void CCComAudio::onEnter()
{
}

void CCComAudio::onExit()
{
    stopBackgroundMusic(true);
    stopAllEffects();
}

bool CCComAudio::isEnabled() const
{
    return m_bEnabled;
}

void CCComAudio::setEnabled(bool b)
{
    m_bEnabled = b;
}

bool CCComAudio::serialize(void* r)
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(r == NULL);
		rapidjson::Value *v = (rapidjson::Value *)r;
		const char *pClassName = DICTOOL->getStringValue_json(*v, "classname");
		CC_BREAK_IF(pClassName == NULL);
		const char *pComName = DICTOOL->getStringValue_json(*v, "name");
		if (pComName != NULL)
		{
			setName(pComName);
		}
		else
		{
			setName(pClassName);
		}
		const rapidjson::Value &fileData = DICTOOL->getSubDictionary_json(*v, "fileData");
		CC_BREAK_IF(!DICTOOL->checkObjectExist_json(fileData));
		const char *pFile = DICTOOL->getStringValue_json(fileData, "path");
		CC_BREAK_IF(pFile == NULL);
		std::string strFilePath;
		if (pFile != NULL)
		{
			strFilePath.assign(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(pFile));
		}
		int nResType = DICTOOL->getIntValue_json(fileData, "resourceType", -1);
		CC_BREAK_IF(nResType != 0);
		if (strcmp(pClassName, "CCBackgroundAudio") == 0)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
			// no MP3 support for CC_PLATFORM_WP8
			std::string::size_type pos = strFilePath.find(".mp3");
			if (pos  == strFilePath.npos)
			{
				continue;
			}
			strFilePath.replace(pos, strFilePath.length(), ".wav");
#endif
			preloadBackgroundMusic(strFilePath.c_str());
			setFile(strFilePath.c_str());
			bool bLoop = DICTOOL->getIntValue_json(*v, "loop") != 0? true:false;
			playBackgroundMusic(strFilePath.c_str(), bLoop);
		}
		else if(strcmp(pClassName, "CCComAudio") == 0)
		{
			preloadEffect(strFilePath.c_str());
		}
		else
		{
			CC_BREAK_IF(true);
		}
		bRet = true;
	} while (0);

	return bRet;
}

CCComAudio* CCComAudio::create(void)
{
    CCComAudio * pRet = new CCComAudio();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
	return pRet;
}

void CCComAudio::end()
{
   CocosDenshion::SimpleAudioEngine::end();
}

void CCComAudio::preloadBackgroundMusic(const char* pszFilePath)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(pszFilePath);
    setFile(pszFilePath);
	setLoop(false);
}

void CCComAudio::playBackgroundMusic(const char* pszFilePath, bool bLoop)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(pszFilePath, bLoop);
}

void CCComAudio::playBackgroundMusic(const char* pszFilePath)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(pszFilePath);
}

void CCComAudio::playBackgroundMusic()
{
	 CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(m_strFilePath.c_str(), m_bLoop);
}

void CCComAudio::stopBackgroundMusic(bool bReleaseData)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(bReleaseData);
}

void CCComAudio::stopBackgroundMusic()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void CCComAudio::pauseBackgroundMusic()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void CCComAudio::resumeBackgroundMusic()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void CCComAudio::rewindBackgroundMusic()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->rewindBackgroundMusic();
}

bool CCComAudio::willPlayBackgroundMusic()
{
    return CocosDenshion::SimpleAudioEngine::sharedEngine()->willPlayBackgroundMusic();
}

bool CCComAudio::isBackgroundMusicPlaying()
{
    return CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying();
}

float CCComAudio::getBackgroundMusicVolume()
{
    return CocosDenshion::SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume();
}

void CCComAudio::setBackgroundMusicVolume(float volume)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(volume);
}

float CCComAudio::getEffectsVolume()
{
    return CocosDenshion::SimpleAudioEngine::sharedEngine()->getEffectsVolume();
}

void CCComAudio::setEffectsVolume(float volume)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(volume);
}

unsigned int CCComAudio::playEffect(const char* pszFilePath, bool bLoop)
{
    return CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(pszFilePath, bLoop);
}

unsigned int CCComAudio::playEffect(const char* pszFilePath)
{
    return CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(pszFilePath);
}

unsigned int CCComAudio::playEffect()
{
	return CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(m_strFilePath.c_str(), m_bLoop);
}

void CCComAudio::pauseEffect(unsigned int nSoundId)
{
    return CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseEffect(nSoundId);
}

void CCComAudio::pauseAllEffects()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

void CCComAudio::resumeEffect(unsigned int nSoundId)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeEffect(nSoundId);
}

void CCComAudio::resumeAllEffects()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}

void CCComAudio::stopEffect(unsigned int nSoundId)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(nSoundId);
}

void CCComAudio::stopAllEffects()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

void CCComAudio::preloadEffect(const char* pszFilePath)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(pszFilePath);
    setFile(pszFilePath);
    setLoop(false);
}

void CCComAudio::unloadEffect(const char *pszFilePath)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect(pszFilePath);
}

void CCComAudio::setFile(const char* pszFilePath)
{
	m_strFilePath.assign(pszFilePath);
}

void CCComAudio::setLoop(bool bLoop)
{
	m_bLoop = bLoop;
}

const char* CCComAudio::getFile()
{
	return m_strFilePath.c_str();
}

bool CCComAudio::isLoop()
{
	return m_bLoop;
}


IMPLEMENT_CLASS_COMPONENT_INFO(CCComRender)
	CCComRender::CCComRender(void)
	: m_pRender(NULL)
{
	m_strName = "CCComRender";
}

CCComRender::~CCComRender(void)
{
	m_pRender = NULL;
}

void CCComRender::onEnter()
{
	if (m_pOwner != NULL)
	{
		m_pOwner->addChild(m_pRender);
	}
}

void CCComRender::onExit()
{
	m_pRender = NULL;
}

cocos2d::CCNode* CCComRender::getNode()
{
	return m_pRender;
}

void CCComRender::setNode(cocos2d::CCNode *pNode)
{
	m_pRender = pNode;
}

bool CCComRender::serialize(void* r)
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(r == NULL);
		rapidjson::Value *v = (rapidjson::Value *)r;
		const char *pClassName = DICTOOL->getStringValue_json(*v, "classname");
		CC_BREAK_IF(pClassName == NULL);
		const char *pComName = DICTOOL->getStringValue_json(*v, "name");
		if (pComName != NULL)
		{
			setName(pComName);
		}
		else
		{
			setName(pClassName);
		}
		const rapidjson::Value &fileData = DICTOOL->getSubDictionary_json(*v, "fileData");
		CC_BREAK_IF(!DICTOOL->checkObjectExist_json(fileData));
		const char *pFile = DICTOOL->getStringValue_json(fileData, "path");
		const char *pPlist = DICTOOL->getStringValue_json(fileData, "plistFile");
		CC_BREAK_IF(pFile == NULL && pPlist == NULL);
		std::string strFilePath;
		std::string strPlistPath;
		if (pFile != NULL)
		{
			strFilePath.assign(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(pFile));
		}
		if (pPlist != NULL)
		{
			strPlistPath.assign(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(pPlist));
		}
		int nResType = DICTOOL->getIntValue_json(fileData, "resourceType", -1);
		if (nResType == 0)
		{
			if (strcmp(pClassName, "CCSprite") == 0 && strFilePath.find(".png") != strFilePath.npos)
			{
				m_pRender = CCSprite::create(strFilePath.c_str());
			}
			else if(strcmp(pClassName, "CCTMXTiledMap") == 0 && strFilePath.find(".tmx") != strFilePath.npos)
			{
				m_pRender = CCTMXTiledMap::create(strFilePath.c_str());
			}
			else if(strcmp(pClassName, "CCParticleSystemQuad") == 0 && strFilePath.find(".plist") != strFilePath.npos)
			{
				m_pRender = CCParticleSystemQuad::create(strFilePath.c_str());
				m_pRender->setPosition(0, 0);
			}
			else if(strcmp(pClassName, "CCArmature") == 0)
			{
				std::string reDir = strFilePath;
				std::string file_path = "";
				size_t pos = reDir.find_last_of('/');
				if (pos != std::string::npos)
				{
					file_path = reDir.substr(0, pos+1);
				}
				rapidjson::Document doc;
				if(!readJson(strFilePath.c_str(), doc))
				{
					CCLog("read json file[%s] error!\n", strFilePath.c_str());
					continue;
				}
				const rapidjson::Value &subData = DICTOOL->getDictionaryFromArray_json(doc, "armature_data", 0);
				const char *name = DICTOOL->getStringValue_json(subData, "name");
				CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(strFilePath.c_str());
				CCArmature *pAr = CCArmature::create(name);
				m_pRender = pAr;
				const char *actionName = DICTOOL->getStringValue_json(*v, "selectedactionname"); 
				if (actionName != NULL && pAr->getAnimation() != NULL)
				{
					pAr->getAnimation()->play(actionName);
				}
			}
			else if(strcmp(pClassName, "GUIComponent") == 0)
			{
				cocos2d::gui::TouchGroup* tg = cocos2d::gui::TouchGroup::create();
				cocos2d::gui::Widget* widget = cocos2d::extension::GUIReader::shareReader()->widgetFromJsonFile(strFilePath.c_str());
				tg->addWidget(widget);
				m_pRender = tg;
			}
			else
			{
				CC_BREAK_IF(true);
			}
		}
		else if (nResType == 1)
		{
			if (strcmp(pClassName, "CCSprite") == 0)
			{
				std::string strPngFile = strPlistPath;
				std::string::size_type pos = strPngFile.find(".plist");
				if (pos  == strPngFile.npos)
				{
					continue;
				}
				strPngFile.replace(pos, strPngFile.length(), ".png");
				CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(strPlistPath.c_str(), strPngFile.c_str());
				m_pRender = CCSprite::createWithSpriteFrameName(strFilePath.c_str());
			}
			else
			{
				CC_BREAK_IF(true);
			}
		}
		else
		{
			CC_BREAK_IF(true);
		}
		bRet = true;
	} while (0);

	return bRet;
}

CCComRender* CCComRender::create(void)
{
	CCComRender * pRet = new CCComRender();
	if (pRet != NULL && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool CCComRender::readJson(const char *pszFileName, rapidjson::Document &doc)
{
	bool bRet = false;
	unsigned long size = 0;
	unsigned char *pBytes = NULL;
	do {
		CC_BREAK_IF(pszFileName == NULL);
		std::string jsonpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pszFileName);
		pBytes = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(jsonpath.c_str(), "r", &size);
		CC_BREAK_IF(pBytes == NULL || strcmp((char*)pBytes, "") == 0);
		CCData *data = new CCData(pBytes, size);
		std::string load_str = std::string((const char *)data->getBytes(), data->getSize() );
		CC_SAFE_DELETE(data);
		CC_SAFE_DELETE_ARRAY(pBytes);
		doc.Parse<0>(load_str.c_str());
		CC_BREAK_IF(doc.HasParseError());
		bRet = true;
	} while (0);
	return bRet;
}

NS_CC_EXT_END

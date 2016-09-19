#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "ui/UIButton.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;

#define GAME_SETTING_MENU_FLAG 10
#define SECOND_MENU_FLAG 11
#define RESOURCE_REQUIREMENT_MENU_FLAG 12
#define FPS_MENU_FLAG 13

#define RESOURCE_PARENT_NODE_FLAG 14

//using ResourceLevel = struct ResourceLevel
//{
//    int nodeNumber;  // for cpu
//    int spriteNumber; // for gpu
//    int actionNumber;
//    int particleNumber;
//    int audioNumber;
//};

// 游戏资源消耗等级1：ID=3；CPU=0,GPU=0
// 游戏资源消耗等级2：ID=3；CPU=1,GPU=1
// 游戏资源消耗等级3：ID=3；CPU=1,GPU=2
// 游戏资源消耗等级4：ID=3；CPU=2,GPU=3
// 游戏资源消耗等级5：ID=3；CPU=2,GPU=4
// 游戏资源消耗等级6：ID=3；CPU=3,GPU=5
// 游戏资源消耗等级7：ID=3；CPU=3,GPU=6
// 游戏资源消耗等级8：ID=3；CPU=4,GPU=7
// 游戏资源消耗等级9：ID=3；CPU=4,GPU=8
// 游戏资源消耗等级10：ID=3；CPU=5,GPU=9

std::vector<HelloWorld::ResourceLevel> HelloWorld::_resourceLevelVector = {
    {0,   0,    0, 0, 0}, // CPU=0,GPU=0
    {25,  25,   25, 10, 5}, // CPU=1,GPU=1
    {0,   50,   0, 0, 0}, // CPU=1,GPU=2
    {200, 100,  0, 0, 0}, // CPU=2,GPU=3
    {100, 200,  0, 0, 0}, // CPU=2,GPU=4
    {200, 100,  0, 0, 0}, // CPU=3,GPU=5
    // todo
    {200, 100,  0, 0, 0}, // CPU=3,GPU=6
    {100, 200,  0, 0, 0}, // CPU=4,GPU=7
    {200, 100,  0, 0, 0}, // CPU=4,GPU=8
    {200, 100,  0, 0, 0}, // CPU=5,GPU=9
};

Scene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    // init _emitter
    _emitter = ParticleSun::create();
    _emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("fire.png"));
    _emitter->setTotalParticles(0);
    _emitter->setPosition(Vec2(100, 100));
    _emitter->pause();
    this->addChild(_emitter);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    // add child to contain resources: node, sprite, particle
    auto resourceParentNode = Node::create();
    resourceParentNode->setTag(RESOURCE_PARENT_NODE_FLAG);
    this->addChild(resourceParentNode);

    // game setting menu
    std::vector<std::string> titles = { "游戏设置" };
    auto listView = this->createListView(titles, Vec2(origin.x + visibleSize.width / 2, origin.y + 50));
    listView->setTag(GAME_SETTING_MENU_FLAG);
    listView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(HelloWorld::gameSettingMenuSelectedItemEvent, this));
    this->addChild(listView);

    // second level menu
    titles = { "选择等级", "切换场景", "帧率选择" };
    listView = this->createListView(titles, Vec2(origin.x + visibleSize.width / 6 * 4, origin.y + 50));
    listView->setTag(SECOND_MENU_FLAG);
    listView->setVisible(false);
    listView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(HelloWorld::secondMenuSelectedItemEvent, this));
    this->addChild(listView);
    
    // resource requirement menu
    titles = { "资源需求等级1", "资源需求等级2", "资源需求等级3", "资源需求等级4", "资源需求等级5",
               "资源需求等级6", "资源需求等级7", "资源需求等级8", "资源需求等级9", "资源需求等级10" };
    listView = this->createListView(titles, Vec2(origin.x + visibleSize.width / 6 * 5, origin.y + 50));
    listView->setTag(RESOURCE_REQUIREMENT_MENU_FLAG);
    listView->setVisible(false);
    listView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(HelloWorld::resourceRequirementMenuSelectedItemEvent, this));
    this->addChild(listView);
    
    // fps menu
    titles = { "25", "30", "40", "60" };
    listView = this->createListView(titles, Vec2(origin.x + visibleSize.width / 6 * 5, origin.y + 50));
    listView->setTag(FPS_MENU_FLAG);
    listView->setVisible(false);
    listView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(HelloWorld::fpsSelectedMenuSelectedItemEvent, this));
    this->addChild(listView);
    
    return true;
}

void HelloWorld::gameSettingMenuSelectedItemEvent(cocos2d::Ref* sender, cocos2d::ui::ListView::EventType type)
{
    if (type == cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END)
    {
        // show second menu
        this->getChildByTag(SECOND_MENU_FLAG)->setVisible(true);
    }
}

void HelloWorld::secondMenuSelectedItemEvent(cocos2d::Ref* sender, cocos2d::ui::ListView::EventType type)
{
    if (type == cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END)
    {
        auto listView = static_cast<ui::ListView*>(sender);
        switch (listView->getCurSelectedIndex()) {
            case 0:
                // 选择等级
                this->getChildByTag(FPS_MENU_FLAG)->setVisible(false);
                this->getChildByTag(RESOURCE_REQUIREMENT_MENU_FLAG)->setVisible(true);
                break;
                
            case 1:
                // 切换场景
            {
                auto scene = HelloWorld::scene();
                Director::getInstance()->replaceScene(scene);
            }
                break;
            case 2:
                // 帧率选择
                this->getChildByTag(RESOURCE_REQUIREMENT_MENU_FLAG)->setVisible(false);
                this->getChildByTag(FPS_MENU_FLAG)->setVisible(true);
                break;
                
            default:
                break;
        }
    }
}

void HelloWorld::resourceRequirementMenuSelectedItemEvent(cocos2d::Ref* sender, cocos2d::ui::ListView::EventType type)
{
    if (type == ui::ListView::EventType::ON_SELECTED_ITEM_END)
    {
        auto listView = static_cast<ui::ListView*>(sender);
        this->addResources(static_cast<int>(listView->getCurSelectedIndex()));
    }
}

void HelloWorld::fpsSelectedMenuSelectedItemEvent(cocos2d::Ref* sender, cocos2d::ui::ListView::EventType type)
{
    if (type == ui::ListView::EventType::ON_SELECTED_ITEM_END)
    {
        auto listView = static_cast<ui::ListView*>(sender);
        switch (listView->getCurSelectedIndex()) {
            case 0:
                // 25
                Director::getInstance()->setAnimationInterval(1 / 25.0);
                break;
            case 1:
                // 30
                Director::getInstance()->setAnimationInterval(1 / 30.0);
                break;
            case 2:
                // 40
                Director::getInstance()->setAnimationInterval(1 / 40.0);
                break;
            case 3:
                // 60
                Director::getInstance()->setAnimationInterval(1 / 60.0);
                break;
                
            default:
                break;
        }
    }
}

cocos2d::ui::ListView* HelloWorld::createListView(const std::vector<std::string>& itemTitles, const cocos2d::Vec2& position)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto listView = ui::ListView::create();
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setPosition(position);
    listView->setContentSize(Size(visibleSize.width / 7, visibleSize.height / 4 * 3));
    
    for (const auto& item: itemTitles)
    {
        auto button = ui::Button::create("button.png", "buttonHighlighted.png");
        button->setTitleText(item);
        button->setScale9Enabled(true);
        button->setContentSize(button->getTitleRenderer()->getContentSize());
        listView->pushBackCustomItem(button);
    }
    
    return listView;
}

void HelloWorld::addResources(int level)
{
    assert(level < 10);
    
    // remove previous resources
    auto resourceParentNode = this->getChildByTag(RESOURCE_PARENT_NODE_FLAG);
    resourceParentNode->removeAllChildren();
    
    // stop all audios
    experimental::AudioEngine::stopAll();
    
    auto resourceLevel = HelloWorld::_resourceLevelVector[level];
    
    // add Nodes
    int nodeNumber = resourceLevel.nodeNumber;
    for (int i = 0; i < nodeNumber; ++i)
        resourceParentNode->addChild(Node::create());
    
    // add Sprites and run actions if needed
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    int spriteNumber = resourceLevel.spriteNumber;
    int actionNumber = resourceLevel.actionNumber;
    for (int i = 0; i < spriteNumber; ++i)
    {
        auto sprite = Sprite::create("grossini.png");
        float x = origin.x + visibleSize.width * (std::rand() * 1.0 / RAND_MAX);
        float y = origin.y + visibleSize.height * (std::rand() * 1.0 / RAND_MAX);
        sprite->setPosition(Vec2(x, y));
        resourceParentNode->addChild(sprite);
        
        if (i < actionNumber)
            sprite->runAction(RepeatForever::create(RotateBy::create(3, 360)));
    }
    
    // add particles
    _emitter->resume();
    _emitter->setTotalParticles(resourceLevel.particleNumber);
    
    // play audioes
    int audioNumber = resourceLevel.audioNumber;
    
    for (int i = 0 ; i < audioNumber; ++i)
    {
        auto audioPath = StringUtils::format("effect%d.mp3", i % 10);
        experimental::AudioEngine::play2d(audioPath.c_str(), true);
    }
}


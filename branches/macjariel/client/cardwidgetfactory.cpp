#include <QWidget>
#include "cardwidgetfactory.h"
#include "game.h"
#include "cardwidget.h"

using namespace client;

CardWidgetFactory::CardWidgetFactory(Game* game):
        mp_game(game)
{
}


CardWidget* CardWidgetFactory::createPlayingCard(QWidget* parent)
{
    CardWidget* cardWidget = new CardWidget(parent, Card::Playing);
    registerCard(cardWidget);
    return cardWidget;
}

CardWidget* CardWidgetFactory::createCharacterCard(QWidget* parent, CharacterType characterType)
{
    CardWidget* cardWidget = new CardWidget(parent, Card::Character);
    registerCard(cardWidget);
    cardWidget->setCharacterType(characterType);
    cardWidget->setSize(CardWidget::SIZE_SMALL);
    return cardWidget;
}

void CardWidgetFactory::registerCard(CardWidget* cardWidget)
{
    cardWidget->setGameObjectClickHandler(mp_game->cardWidgetClickHandler());
}


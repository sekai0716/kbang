#ifndef PLAYERCTRL_H
#define PLAYERCTRL_H

#include <QObject>

#include "gameexceptions.h"
#include "parser/parserstructs.h"

class Player;
class GameEventHandler;
class PublicGameView;
class PublicPlayerView;
class PrivatePlayerView;
class PlayingCard;

/**
 * This class provides the only "correct" form to
 * control a player. It means that any code that
 * wants to control a player needs to do it throught
 * this API. It's used primarily by AIs and the
 * adaptor between this class and xml parser.
 */
class PlayerCtrl :  QObject
{
    Q_OBJECT;
    friend class Player;
public slots:
    /**
     * Disconnects the player from the game, or possibly subtitutes player with
     * AI. This method should never be called by AIs
     * Typically you want to call this method after the remote client
     * disconnected or requested a disconnection.
     */
    void disconnect();

    /**
     * Starts the yet not started game.
     * \throws BadPlayerException The player does not have the permission to
     *         start the game.
     * \throws BadGameStateException Either the game is already started, or
     *         the right conditions are not met, eg. there is not enough
     *         players in the game.
     */
    void startGame();


    /**
     * Player draws the specified number of cards. This is how player starts it turn.
     * \param numCards The amount of cards to draw.
     * \param revealCard The player reveals the drawed card(s). This is necessary for
     *        some characters to gain bonuses.
     * The player typically draws two cards. Client class can either call drawCard once
     * with the numCards=2 or twice with numCards=1. Moreover, in case of special characters
     * (drawing from graveyard, from other hands, ...) the other variant becomes handy.
     *
     * \throws BadGameStateException It's not right time to draw cards or the card count is bad.
     */
    void drawCard(int numCards, bool revealCard = 0);

    /**
     * Finishes the player's turn.
     * \throws TooManyCardsInHandException The player must discard some cards,
     *         before he can finish his turn.
     * \throws BadGameStateException The player cannot finish his turn, because
     *         it's not the right moment.
     */
    void finishTurn();

    /**
     * Discards a card.
     * \param cardId The id of the card.
     * \throws BadCardException There is no card with given id in player's hand.
     * \throws BadGameStateException There is no right moment for discarding cards.
     */
    void discardCard(PlayingCard* card);



    /**
     * Plays a card.
     */
    void playCard(PlayingCard* card);
    void playCard(PlayingCard* card, const PublicPlayerView* player);


    void pass();

public:



    const PublicGameView& publicGameView() const;
    const PublicPlayerView& publicPlayerView(int playerId = 0) const;
    const PrivatePlayerView& privatePlayerView() const;



    /**
     * Creates a new game and automatically puts the player in. This should be used
     * exclusively by nonAI client classes, as AIs typically don't create games.
     * \note The client class gets the PlayerCtrl instance through the
     *       GameEventHandler::onPlayerInit(PlayerCtrl*) method.
     * \param game The game structure.
     * \param player The player structure.
     */
    static void createGame(const StructGame& game, const StructPlayer& player,
                                  GameEventHandler* gameEventHandler);

    /**
     * Puts the player to the game.
     * \note The client class gets the PlayerCtrl instance through the
     *       GameEventHandler::onPlayerInit(PlayerCtrl*) method.
     *
     * \param gameId The id of the game.
     * \param player The player structure.
     * \throws BadGameException The game does not exist.
     * \throws BadGameStateException The game has already started.
     */
    static void joinGame(int gameId, const StructPlayer& player,
                                GameEventHandler* gameEventHandler);



    /**
     * Returns the StructServerInfo struct.
     */
    static StructServerInfo structServerInfo();

    /**
     * Returns the list of PublicGameViews.
     */
    static QList<const PublicGameView*> publicGameList();

    /**
     * Returns the PublicGameView of the game specified by gameId.
     */
    static const PublicGameView& publicGameView(int gameId);


private:
    PlayerCtrl(Player* player);
    PlayerCtrl(const PlayerCtrl&): QObject(0) {}
    PlayerCtrl& operator=(const PlayerCtrl&) { return *this; }

    Player*                     mp_player;

};

#endif // PLAYERCTRL_H

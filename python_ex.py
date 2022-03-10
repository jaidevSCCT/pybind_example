from pybind_tutorial import SportsPlayer, multipleArgsFunction
import numpy as np


class PythonObject:
    def __init__(self):
        self.playerName = ""
        self.sportsType = ""
        self.playerAge = 0

    def __str__(self) -> str:
        return f'player name={self.playerName} , sports type={self.sportsType}  , ' \
               f'player age={self.playerAge}'


def call_sports_player():
    player_1 = {"player name": "R Jadeja", "sports type": "Cricket", "player age": 33}
    player_2 = {"player name": "C Ronaldo", "sports type": "Football", "player age": 37}
    player_3 = {"player name": "R Federer", "sports type": "Tennis", "player age": 40}
    player_obj = SportsPlayer("", "", 0)
    player_obj.addPlayers(player_1)
    player_obj.addPlayers(player_2)
    player_obj.addPlayers(player_3)
    ret_list = player_obj.getPlayersList()
    print(ret_list)


if __name__ == "__main__":
    call_sports_player()
    my_obj = PythonObject()
    my_obj.playerName = 'R Ashwin'
    my_obj.sportsType = 'Cricket'
    my_obj.playerAge = 34
    nparray = np.arange(20).reshape(4, 5)
    my_tuple = ('Lettuce', 'Apple', float(55.8))
    return_tuple = multipleArgsFunction(nparray, my_obj, my_tuple)
    print(return_tuple)

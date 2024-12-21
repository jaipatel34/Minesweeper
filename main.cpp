
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>

#include "RandomNew.h"
#include "ThreeNew.h"
#include <algorithm>
#include <fstream>
#include <unordered_map>


using namespace std;

class Tile{
public:

    sf::Sprite showSprite;
    sf::Sprite mineSprite;
    sf::Sprite flagSprite;
    sf::Sprite numSprite;

    sf::Texture texture;
    sf::Texture mineTexture;
    sf::Texture flagTexture;

    bool mine;
    bool num;
    bool flags;
    bool show;
    int value;
    int neighbor_mines;


    Tile(){
        texture.loadFromFile("files/images/tile_hidden.png");
        showSprite = sf::Sprite(texture);

        flags = false;
        mine = false;
        num = false;
        show = false;
        neighbor_mines = 0;
        value = 0;

    }

    Tile(sf::Vector2f positionSet,string textureName){
        texture.loadFromFile(textureName);
        showSprite = sf::Sprite(texture);
        showSprite.setPosition(positionSet.x, positionSet.y);

        mine = false;
        flags = false;
        show = false;

    }
    void SetNumber(sf::Texture& textureName){
        num = true;
        numSprite = sf::Sprite(textureName);



    }

    void SetFlag(){
        flags = !flags;
        flagTexture.loadFromFile("files/images/flag.png");
        flagSprite = sf::Sprite(flagTexture);


    }

    void SetMine(){

        value = 1;
        mine = true;

        mineTexture.loadFromFile("files/images/mine.png");
        mineSprite = sf::Sprite(mineTexture);

    }



    void SetShow(sf::Texture& textureName){
        show = true;
        showSprite = sf::Sprite(textureName);

    }



};



class Board{
public:
    int cols;
    int rows;
    int mines;
    vector<vector<Tile>> tiles;


    Board(int col, int row,  int mine){
        cols = col;
        rows = row;
        mines = mine;

        tiles.resize(cols);

        for(int i = 0; i<cols; i++){
            tiles[i].resize(rows);
        }


        int mineTotal = 0;

        while (mines>mineTotal){

            int random_x = Random::RandNum(0, cols - 1);
            int random_y = Random::RandNum(0, rows - 1);


            if (!tiles[random_x][random_y].mine){
                tiles[random_x][random_y].SetMine();
                mineTotal++;
            }
        }

        for (int i= 0; i <cols; i++){
            for (int j = 0; j < rows; j++){
                showNeighborMines(i, j);
            }
        }

    }
    void showNeighborMines(int x, int y){
        int total = 0;

        for (int i = x - 1; i <= x + 1; i++) {
            for (int j = y - 1; j <= y + 1; j++) {

                if (i >= 0 && j >= 0 && i < cols && j < rows && tiles[i][j].mine) {
                    total++;
                }
            }
        }

        tiles[x][y].neighbor_mines = total;
    }

    void show_neighbors(sf::Texture shows,sf::Texture numb, int x,int y){

        for (int i = x - 1; i <= x +1;i++) {
            for (int j =y - 1; j <=y + 1; j++) {
                if (i >= 0 &&  j >= 0 &&i < cols && j < rows && !tiles[i][j].show &&!tiles[i][j].mine && !tiles[i][j].flags) {
                    if (tiles[i][j].num) {
                        tiles[i][j].SetNumber(numb);
                        tiles[i][j].SetShow(shows);
                    }
                    else{
                        tiles[i][j].SetShow(shows);
                        show_neighbors(numb, numb, i, j);
                    }

                }
            }
        }

    }




    void showArea(sf::Texture &textureName,unordered_map<int, sf::Texture> &numb, int x, int y){
        for (int i = x -1; i <= x + 1; i++) {
            for (int j = y - 1; j<= y + 1; j++) {
                if (i >= 0 && j >= 0 && i< cols && j <rows && !tiles[i][j].show && !tiles[i][j].flags && !tiles[i][j].mine) {
                    tiles[i][j].SetShow(textureName);
                    if (tiles[i][j].neighbor_mines != 0){
                        tiles[i][j].SetNumber(numb[tiles[i][j].neighbor_mines]);
                    }
                    else{
                        showArea(textureName, numb, i, j);
                    }
                }
            }
        }
    }
    bool CheckWin() {
        for (int i = 0;i < tiles.size(); i++) {
            for (int j = 0; j< tiles.at(i).size(); j++) {
                if (!tiles[i][j].mine &&!tiles[i][j].show) {
                    return false;
                }
            }
        }
        return true;
    }

    void ResetBoard(sf::Texture& original,int col, int row, int mine){
        cols = col;
        rows = row;
        mines = mine;

        for (int i = 0; i < cols; i++){
            for(int j = 0; j <rows; j++){
                tiles[i][j].mineSprite.setTexture(original);
                tiles[i][j].showSprite.setTexture(original);
                tiles[i][j].numSprite.setColor(sf::Color::Transparent);
                tiles[i][j].flags = false;
                tiles[i][j].mine = false;
                tiles[i][j].num = false;
                tiles[i][j].show = false;
                tiles[i][j].neighbor_mines = 0;

            }
        }

        int mineTotal = 0;

        while (mines> mineTotal){
            int rand_x = Random::RandNum(0, cols - 1);
            int rand_y = Random::RandNum(0, rows - 1);

            if (!tiles[rand_x][rand_y].mine){
                tiles[rand_x][rand_y].SetMine();
                mineTotal++;
            }
        }

        for (int i = 0; i < cols; i++){
            for (int j = 0; j <rows; j++){
                showNeighborMines(i, j);
            }
        }

    }


};



string printLeaderBoard(map<string, string> &player){
    string players = "";
    int index = 1;
    auto iter = player.begin();
    for (auto iter = player.begin(); iter != player.end(); iter++){
        players.append(to_string(index) + ". \t" + iter->first + "\t" + iter->second + "\n\n");
        index++;
    }

    return players;
}
void setText(sf::Text &text,float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}



int main() {
//
    bool around = true;
    sf::Font font;

    bool cursorVisible = true;
    sf::Clock cursorClock;


    bool win = false;
    bool lose = false;
    bool debug_button = false;
    bool pause_button = false;
    bool leader = false;
    int flag_count = 0;
    bool reset = false;

    int flags_placed = 0;
    int showCount = 0;

    bool nega_tive=false;
    int minutes;
    int seconds;
    bool hasWon = false;
    int counterVal;
    int newVal1;
    int newVal2;
    int newVal3;

    unordered_map<int, sf::Texture> number;
    number[1].loadFromFile("files/images/number_1.png");
    number[2].loadFromFile("files/images/number_2.png");
    number[3].loadFromFile("files/images/number_3.png");
    number[4].loadFromFile("files/images/number_4.png");
    number[5].loadFromFile("files/images/number_5.png");
    number[6].loadFromFile("files/images/number_6.png");
    number[7].loadFromFile("files/images/number_7.png");
    number[8].loadFromFile("files/images/number_8.png");

    unordered_map<string, sf::Texture> textures;
    textures["mine"].loadFromFile("files/images/mine.png");
    textures["unrevealed"].loadFromFile("files/images/tile_hidden.png");
    textures["show"].loadFromFile("files/images/tile_revealed.png");
    textures["flag"].loadFromFile("files/images/flag.png");
    textures["play"].loadFromFile("files/images/play.png");
    textures["pause"].loadFromFile("files/images/pause.png");
    textures["win"].loadFromFile("files/images/face_win.png");
    textures["sad"].loadFromFile("files/images/face_lose.png");
    textures["face_happy"].loadFromFile("files/images/face_happy.png");


    unordered_map<int, sf::Texture> time;
    time[0].loadFromFile("files/images/digits.png", sf::IntRect(0, 0, 21, 32));
    time[1].loadFromFile("files/images/digits.png", sf::IntRect(21, 0, 21, 32));
    time[2].loadFromFile("files/images/digits.png", sf::IntRect(42, 0, 21, 32));
    time[3].loadFromFile("files/images/digits.png", sf::IntRect(63, 0, 21, 32));
    time[4].loadFromFile("files/images/digits.png", sf::IntRect(84, 0, 21, 32));
    time[5].loadFromFile("files/images/digits.png", sf::IntRect(105, 0, 21, 32));
    time[6].loadFromFile("files/images/digits.png", sf::IntRect(126, 0, 21, 32));
    time[7].loadFromFile("files/images/digits.png", sf::IntRect(147, 0, 21, 32));
    time[8].loadFromFile("files/images/digits.png", sf::IntRect(168, 0, 21, 32));
    time[9].loadFromFile("files/images/digits.png", sf::IntRect(189, 0, 21, 32));
    time[10].loadFromFile("files/images/digits.png", sf::IntRect(210, 0, 21, 32));

    ifstream file("files/config.cfg");

    string column;
    string row;
    string mines;


    getline(file, column);
    getline(file, row);
    getline(file, mines);



    int columnNew = stoi(column);
    int rowNew = stoi(row);
    int minesNew = stoi(mines);

    bool gameWindowOpen= false;




    sf::RenderWindow window(sf::VideoMode(((columnNew)*32)+100, (rowNew) * 32), "Welcome Window", sf::Style::Close);


    if (!font.loadFromFile("files/font.ttf")) {
        cout << "Error" << endl;
    }


    sf::Text text("WELCOME TO MINESWEEPER!", font, 24);
    setText(text, (window.getSize().x) / 2, ((window.getSize().y) / 2) - 150);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::Text text2("Enter your name:", font, 20);
    setText(text2, (window.getSize().x) / 2, ((window.getSize().y) / 2) - 75);
    text2.setFillColor(sf::Color::White);
    text2.setStyle(sf::Text::Bold);

    sf::Text text3("", font, 18);
    setText(text3, (window.getSize().x) / 2, ((window.getSize().y) / 2 - 45));
    text3.setFillColor(sf::Color::Yellow);

    sf::Text cursor("|", font, 20);
    cursor.setFillColor(sf::Color::Yellow);
    cursor.setStyle(sf::Text::Bold);

    string output;

    while (window.isOpen()) {
        sf::Event event;
        window.clear(sf::Color::Blue);

        window.draw(text);
        window.draw(text2);
        window.draw(text3);

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::TextEntered) {
                if (isalpha(event.text.unicode) && text3.getString().getSize() < 10) {
                    if (output.empty()) {
                        output += std::toupper(static_cast<char>(event.text.unicode));
                    }
                    else {
                        output += std::tolower(static_cast<char>(event.text.unicode));
                    }
                    text3.setString(output);
                    setText(text3, (window.getSize().x) / 2, ((window.getSize().y) / 2 - 45));

                }
                else if (event.text.unicode == '\b' && !text3.getString().isEmpty()) {
                    output.pop_back();
                    text3.setString(output);
                    setText(text3, (window.getSize().x) / 2, ((window.getSize().y) / 2 - 45));

                }
                else if (event.text.unicode == '\r' && !text3.getString().isEmpty()) {
                    gameWindowOpen = true;
                    window.close();
                }
            }
        }
        if (cursorClock.getElapsedTime().asSeconds() > 0.5) {
            cursorVisible = !cursorVisible;
            cursorClock.restart();
        }
        if (cursorVisible) {
            sf::FloatRect inputRect = text3.getLocalBounds();
            setText(cursor, (window.getSize().x) / 2 + inputRect.width * 0.6, ((window.getSize().y) / 2 - 45));
            window.draw(cursor);
        }
        window.display();

    }


    ifstream leaderboard_file("files/leaderboard.txt");
    string line1;
    string line2;
    string line3;
    string line4;
    string line5;

    getline(leaderboard_file, line1);
    getline(leaderboard_file, line2);
    getline(leaderboard_file, line3);
    getline(leaderboard_file, line4);
    getline(leaderboard_file, line5);

    string name1 = line1.substr(line1.find(",") + 1, -1);
    string clock1 = line1.substr(0, line1.find(","));
    string name2 = line2.substr(line2.find(",") + 1, -1);
    string clock2 = line2.substr(0, line2.find(","));
    string name3 = line3.substr(line3.find(",") + 1, -1);
    string clock3 = line3.substr(0, line3.find(","));
    string name4 = line4.substr(line4.find(",") + 1, -1);
    string clock4 = line4.substr(0, line4.find(","));
    string name5 = line5.substr(line5.find(",") + 1, -1);
    string clock5 = line5.substr(0, line5.find(","));


    string final_name= output+"*";


    map<string, string> playerTimeName;
    playerTimeName.emplace(clock1, name1);
    playerTimeName.emplace(clock2, name2);
    playerTimeName.emplace(clock3, name3);
    playerTimeName.emplace(clock4, name4);
    playerTimeName.emplace(clock5, name5);



    int countVal1;
    int countVal2;
    int countVal3;
    int digit;
    if (minesNew >=100){
        digit = 3;
        countVal1 = stoi(mines.substr(0, 1));
        countVal2 = stoi(mines.substr(1, 2));
        countVal3 = stoi(mines.substr(2, -1));
    }

    else if (minesNew >=10 && minesNew <= 99){
        digit = 2;
        countVal1 = 0;
        countVal2 = stoi(mines.substr(0, 1));
        countVal3 = stoi(mines.substr(1, -1));

    }
    else if (minesNew >= 0 &&minesNew <= 9){
        digit = 1;
        countVal3 = stoi(mines.substr(0, -1));
        countVal2 = 0;
        countVal1 = 0;
    }


    auto PauseTime = chrono::high_resolution_clock::now();
    auto startTime = chrono::high_resolution_clock::now();
    auto ElpasedTime = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - PauseTime).count();

    sf::RenderWindow gameWindow(sf::VideoMode((columnNew)*32, ((rowNew)*32)+100), "Minesweeper",sf::Style::Close | sf::Style::Titlebar);
    Board board(columnNew, rowNew, minesNew);


    Buttons negative(time[10], 12,32 * ((rowNew) + 0.5) + 16);
    Buttons count1(time[countVal1],33, 32 * ((rowNew) + 0.5) + 16);
    Buttons count2(time[countVal2],54, 32 * ((rowNew) + 0.5) + 16);
    Buttons count3(time[countVal3], 75, 32 * ((rowNew) + 0.5) + 16);

    sf::Texture debugg;
    debugg.loadFromFile("files/images/debug.png");
    Buttons debug(debugg, (((columnNew) * 32) - 304), 32 * (rowNew + 0.5));

    sf::Texture face_happy;
    face_happy.loadFromFile("files/images/face_happy.png");
    Buttons happy(face_happy, (((columnNew) / 2.0) * 32) - 32, 32 * (rowNew + 0.5));


    sf::Texture leaderboardd;
    leaderboardd.loadFromFile("files/images/leaderboard.png");
    Buttons leaderboardButton(leaderboardd, (((columnNew) * 32) - 176), 32 * (rowNew + 0.5));

    sf::Texture pausee;
    pausee.loadFromFile("files/images/pause.png");
    Buttons pause(pausee, (((columnNew) * 32) - 240), 32 * (rowNew + 0.5));


    sf::Texture timeDigits;
    timeDigits.loadFromFile("files/images/digits.png", sf::IntRect(0, 0, 21, 32));
    Buttons minCol1(timeDigits, (columnNew) * 32 - 97, 32 * ((rowNew) + 0.5) + 16);
    Buttons minCol2(timeDigits, (columnNew) * 32 - 76, 32 * ((rowNew) + 0.5) + 16);
    Buttons secCol1(timeDigits, (columnNew) * 32 - 54, 32 * ((rowNew) + 0.5) + 16);
    Buttons secCol2(timeDigits, (columnNew) * 32 - 34, 32 * ((rowNew) + 0.5) + 16);






    if (gameWindowOpen) {
        while (gameWindow.isOpen()) {
            gameWindow.clear(sf::Color::White);

            auto runTime = std::chrono::duration_cast<std::chrono::seconds>(chrono::high_resolution_clock::now() - startTime);
            int totalTime = runTime.count();

            if (board.CheckWin()) {
                win = true;
                hasWon = true;

            }

            int minute_left = minutes / 10 % 10;
            int minute_right = minutes % 10;
            int second_left = seconds / 10 % 10;
            int second_right = seconds % 10;

            string final_second = to_string(seconds);
            string final_minute = to_string(minutes);

            if (!pause_button) {
                totalTime = totalTime - ElpasedTime;
                minutes = totalTime / 60;
                seconds = totalTime % 60;
            }
            if (final_second.length()< 2) {
                final_second = "0" + final_second;
            }
            if (final_minute.length() < 2) {
                final_minute = "0" + final_minute;

            }


            string final_time = final_minute + ":" + final_second;



            for (int i = 0; i <board.cols;i++) {
                for (int j = 0; j< board.rows; j++) {
                    board.tiles[i][j].showSprite.setPosition(32 * i, 32 *j);
                    gameWindow.draw(board.tiles[i][j].showSprite);
                    auto currTile = board.tiles[i][j];


                    if (currTile.show == true &&currTile.mine == true && !pause_button) {
                        currTile.mineSprite.setPosition(32 * i, 32 * j);
                        gameWindow.draw(currTile.mineSprite);
                    }
                    else if (!currTile.mine&& currTile.show && !pause_button ) {
                        currTile.numSprite.setPosition(32 * i, 32 * j);
                        gameWindow.draw(currTile.numSprite);
                    }
                    if (pause_button) {
                        sf::Sprite temp(textures["show"]);
                        temp.setPosition(currTile.showSprite.getPosition());
                        gameWindow.draw(temp);
                    }
                    if (currTile.flags) {
                        currTile.flagSprite.setPosition(32 * i, 32 * j);
                        gameWindow.draw(currTile.flagSprite);
                    }
                    if (!win && !lose &&debug_button) {
                        currTile.mineSprite.setPosition(32 * i, 32 * j);
                        gameWindow.draw(currTile.mineSprite);
                    }



                    if (win) {

                        happy.SetTexture(textures["win"]);



                        if (!currTile.flags &&currTile.mine) {
                            currTile.SetFlag();
                            currTile.flagSprite.setPosition(32 * i, 32 * j);
                            gameWindow.draw(currTile.flagSprite);

                        }


                        if (hasWon&& around) {
                            ofstream leader_file("files/leaderboard.txt", ios_base::out);

                            hasWon = false;
                            around = false;
                            playerTimeName.emplace(final_time, final_name);


                            for (auto iter = playerTimeName.begin(); iter!= playerTimeName.end(); iter++) {

                                if (playerTimeName.size()>5) {
                                    playerTimeName.erase(--playerTimeName.end());
                                }
                                if (iter->second.find("*") != string::npos) {
                                    playerTimeName[iter->first] = iter->second.substr(0, iter->second.size());

                                }


                                leader_file.write((iter->first + "," + iter->second + "\n").c_str(),(iter->first + "," + iter->second + "\n").length());


                            }

                            sf::RenderWindow leaderboard(sf::VideoMode(16 * (columnNew), (16 * rowNew) + 50),"Leaderboard", sf::Style::Close | sf::Style::Titlebar);

                            sf::Text text4((printLeaderBoard(playerTimeName)),font,18);
                            text4.setStyle(sf::Text::Bold);
                            text4.setFillColor(sf::Color::White);
                            setText(text4, 16 * (columnNew) / 2.0, 17 * (rowNew) / 2.0+ 20);

                            sf::Text text5("LEADERBOARD",font,20);
                            text5.setFillColor(sf::Color::White);
                            text5.setStyle(sf::Text::Underlined | sf::Text::Bold);
                            setText(text5, 16*(columnNew)/ 2.0,  17* (rowNew)/ 2.0 - 120);




                            while (leaderboard.isOpen()) {

                                sf::Event event2;
                                while (leaderboard.pollEvent(event2)) {
                                    if (event2.type == sf::Event::Closed) {
                                        hasWon =false;
                                        leaderboard.close();
                                    }
                                }
                                leaderboard.clear(sf::Color::Blue);
                                leaderboard.draw(text5);
                                leaderboard.draw(text4);
                                leaderboard.display();
                            }


                        }


                    }
                    else if (lose) {
                        happy.SetTexture(textures["sad"]);
                        if (currTile.mine && !currTile.flags) {
                            currTile.showSprite.setTexture(textures["show"]);
                            currTile.mineSprite.setPosition(32 * i, 32 * j);
                            currTile.showSprite.setPosition(32 * i, 32 * j);
                            gameWindow.draw(currTile.showSprite);
                            gameWindow.draw(currTile.mineSprite);

                        }

                    }

                    if (reset) {
                        reset = false;
                        around =true;

                        if (win) {
                            win= false;
                            happy.SetTexture(textures["face_happy"]);
                        }
                        else {
                            lose = false;
                            happy.SetTexture(textures["face_happy"]);
                        }
                    }


                }
            }

            sf::Event event3;

            while (gameWindow.pollEvent(event3)) {
                if (event3.type== sf::Event::Closed) {
                    gameWindow.close();
                }

                if (event3.type == sf::Event::MouseButtonPressed) {

                    sf::Mouse mouse;
                    auto pos = mouse.getPosition(gameWindow);

                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        auto HappyRect= happy.open().getGlobalBounds();
                        if (HappyRect.contains(pos.x, pos.y)) {
                            showCount = 0;

                            flag_count = 0;
                            flags_placed= 0;

                            reset = true;
                            pause_button = false;
                            nega_tive = false;


                            startTime = chrono::high_resolution_clock::now();

                            board.ResetBoard(textures["unrevealed"], columnNew, rowNew, minesNew);
                            happy.SetTexture(textures["face_happy"]);


                            count1.SetTexture(time[countVal1]);
                            count2.SetTexture(time[countVal2]);
                            count3.SetTexture(time[countVal3]);
                        }
                        auto LeaderboardRect= leaderboardButton.open().getGlobalBounds();
                        if (LeaderboardRect.contains(pos.x, pos.y)) {
                            leader = !leader;
                            for (int i = 0; i < board.cols; i++) {
                                for (int j = 0; j < board.rows; j++) {
                                    sf::Sprite temp(textures["show"]);
                                    temp.setPosition(board.tiles[i][j].showSprite.getPosition());
                                    gameWindow.draw(temp);
                                }
                            }
                            gameWindow.draw(secCol1.open());
                            gameWindow.draw(secCol2.open());
                            gameWindow.draw(minCol1.open());
                            gameWindow.draw(minCol2.open());



                            if (leader) {
                                PauseTime = chrono::high_resolution_clock::now();

                                sf::RenderWindow leaderboard(sf::VideoMode(16 * columnNew, (16 * rowNew) + 50),"Leaderboard", sf::Style::Close | sf::Style::Titlebar);

                                sf::Text text4((printLeaderBoard(playerTimeName)),font,18);
                                text4.setStyle(sf::Text::Bold);
                                text4.setFillColor(sf::Color::White);
                                setText(text4, 16 * columnNew / 2.0, 17 * rowNew / 2.0+ 20);

                                sf::Text text5("LEADERBOARD",font,20);
                                text5.setFillColor(sf::Color::White);
                                text5.setStyle(sf::Text::Underlined | sf::Text::Bold);
                                setText(text5, 16*columnNew / 2.0,  17*rowNew / 2.0 - 120);

                                while (leaderboard.isOpen()) {
                                    sf::Event event2;
                                    while (leaderboard.pollEvent(event2)) {
                                        if (event2.type == sf::Event::Closed) {
                                            leader =false;
                                            leaderboard.close();
                                            auto resumeTime= chrono::high_resolution_clock::now();
                                            ElpasedTime = ElpasedTime += chrono::duration_cast<chrono::seconds>(resumeTime - PauseTime).count();

                                        }
                                    }
                                    leaderboard.clear(sf::Color::Blue);
                                    leaderboard.draw(text5);
                                    leaderboard.draw(text4);
                                    leaderboard.display();

                                }

                            }
                            if (nega_tive) {
                                gameWindow.draw(negative.open());
                            }
                            gameWindow.draw(leaderboardButton.open());
                            gameWindow.draw(pause.open());
                            gameWindow.draw(happy.open());
                            gameWindow.draw(count1.open());
                            gameWindow.draw(count2.open());
                            gameWindow.draw(count3.open());
                            gameWindow.draw(debug.open());
                            gameWindow.display();
                        }


                        if (!lose && !win) {
                            auto PauseRect= pause.open().getGlobalBounds();
                            if (PauseRect.contains(pos.x, pos.y)) {
                                pause_button =!pause_button;
                                if (pause_button) {
                                    PauseTime = chrono::high_resolution_clock::now();

                                    pause.SetTexture(textures["play"]);


                                }
                                else {
                                    auto resumeTime= chrono::high_resolution_clock::now();
                                    pause.SetTexture(textures["pause"]);
                                    ElpasedTime = ElpasedTime += chrono::duration_cast<chrono::seconds>(resumeTime - PauseTime).count();

                                }

                            }


                            if (!pause_button) {
                                for (int i = 0; i < board.cols;i++) {
                                    for (int j =0; j< board.rows; j++) {
                                        auto Tile_rect = board.tiles[i][j].showSprite.getGlobalBounds();
                                        if (Tile_rect.contains(pos.x, pos.y)) {
                                            if (!board.tiles[i][j].flags && board.tiles[i][j].mine) {
                                                board.tiles[i][j].SetShow(textures["show"]);
                                                lose = true;

                                            }
                                            else if (!board.tiles[i][j].flags) {

                                                if (board.tiles[i][j].neighbor_mines== 0) {
                                                    board.showArea(textures["show"], number, i, j);
                                                }
                                                else if (board.tiles[i][j].neighbor_mines!= 0) {
                                                    board.tiles[i][j].SetNumber(number[board.tiles[i][j].neighbor_mines]);
                                                    board.tiles[i][j].SetShow(textures["show"]);

                                                }

                                            }
                                        }
                                    }
                                }

                                auto DebugRect = debug.open().getGlobalBounds();
                                if (DebugRect.contains(pos.x, pos.y)) {
                                    debug_button =!debug_button;
                                }

                            }
                        }
                    }
                    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                        if (!lose &&!win) {
                            for (int i = 0; i < board.cols; i++) {
                                for (int j = 0; j < board.rows; j++) {
                                    auto FlagRect= board.tiles[i][j].showSprite.getGlobalBounds();
                                    if (!board.tiles[i][j].show&&FlagRect.contains(pos.x, pos.y)) {
                                        if (board.tiles[i][j].flags) {
                                            flags_placed--;
                                        }
                                        else {
                                            flags_placed++;
                                        }

                                        board.tiles[i][j].SetFlag();
                                        counterVal = minesNew - flags_placed;


                                        if (counterVal< 0) {
                                            nega_tive = true;
                                            newVal1 = -1 * (counterVal / 100);
                                            newVal2 = -1 * (counterVal / 10);
                                            newVal3 = -1 * (counterVal % 10);
                                        }
                                        else {
                                            nega_tive =false;
                                            if (digit== 1) {
                                                newVal3 = minesNew - flags_placed;
                                            }
                                            else if (digit ==2) {
                                                newVal2 = counterVal / 10;
                                                newVal3 = counterVal % 10;

                                            }
                                            else if (digit== 3) {
                                                newVal1 = counterVal / 100;
                                                newVal2 = (counterVal % 100) / 10;
                                                newVal3 = counterVal % 10;
                                            }

                                        }
                                        count1.SetTexture(time[newVal1]);
                                        count2.SetTexture(time[newVal2]);
                                        count3.SetTexture(time[newVal3]);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (nega_tive) {
                gameWindow.draw(negative.open());
            }

            if (!lose &&!win) {

                minCol1.SetTexture(time[minute_left]);
                minCol2.SetTexture(time[minute_right]);
                secCol1.SetTexture(time[second_left]);
                secCol2.SetTexture(time[second_right]);

            }



            gameWindow.draw(count1.open());
            gameWindow.draw(count2.open());
            gameWindow.draw(count3.open());
            gameWindow.draw(secCol2.open());
            gameWindow.draw(secCol1.open());
            gameWindow.draw(pause.open());
            gameWindow.draw(minCol2.open());
            gameWindow.draw(minCol1.open());
            gameWindow.draw(happy.open());
            gameWindow.draw(leaderboardButton.open());
            gameWindow.draw(debug.open());
            gameWindow.display();

        }

    }
};
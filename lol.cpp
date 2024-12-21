




/*
07:01,Alex
09:04,Bruce
10:03,Carl
11:24,Daniel
12:15,Eve




#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <random>
#include "RandomNew.h"
#include "ButtonNew.h"
#include <fstream>
#include <unordered_map>
#include <chrono>


using namespace std;

class Tile{
public:

    //sf::Vector2f positionSet;

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

    int neighbor_mines;
    int value;  // 1 for mine and 0 for not a mine




    Tile(sf::Vector2f positionSet, string textureName){
        texture.loadFromFile(textureName);
        showSprite = sf::Sprite(texture);
        showSprite.setPosition(positionSet.x, positionSet.y);

        mine = false;
        flags = false;
        show = false;


    }

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
int numRevealedTiles;
bool gameWon;
bool gameOver;


Board(int col, int row, int mine){
    cols = col;
    rows = row;
    mines = mine;

    tiles.resize(cols);         // fill vector with tiles which by default are not mines

    for(int i = 0; i < cols; i++){
        tiles[i].resize(rows);
    }

    // place mines in places according to random positions
    int mineTotal = 0;

    while (mines>mineTotal){
        // generate a random x and y position using Random.h
        int random_x = Random::Int(0, cols - 1);
        int random_y = Random::Int(0, rows - 1);

        // add Tile at randomized position if it is not already a mine
        if (!tiles[random_x][random_y].mine){
            tiles[random_x][random_y].SetMine();
            mineTotal++;
        }
    }

    for (int i = 0; i < cols; i++){         // determine all the neighbors for each of the individual tiles
        for (int j = 0; j < rows; j++){
            showNeighborMines(i, j);
        }
    }



    //cout << "END" << endl;
}
void showNeighborMines(int x, int y){
    int total = 0;

    // Iterate over neighboring tiles in a 3x3 grid around the current tile
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            // Check if the current neighbor tile has a mine and is within the bounds of the board
            if (i >= 0 && j >= 0 && i < cols && j < rows && tiles[i][j].mine) {
                total++;
            }
        }
    }

    tiles[x][y].neighbor_mines = total;
}
void show_neighbors(sf::Texture shows,sf::Texture numb,  int x, int y){     // pass in the position of the mine you want to check
    //int count = 0;

    // Iterate over neighboring tiles in a 3x3 grid around the current tile
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            // Check if the current neighbor tile has a mine and is within the bounds of the board
            if (i >= 0 &&  j >= 0 && i < cols && j < rows && !tiles[i][j].show && !tiles[i][j].flags && !tiles[i][j].mine) {
                if (tiles[i][j].num) {
                    tiles[i][j].SetShow(shows);
                    tiles[i][j].SetNumber(numb);
                }
                else{
                    tiles[i][j].SetShow(shows);
                    show_neighbors(numb, numb, i, j);
                }

            }
        }
    }

}




void showArea(sf::Texture &textureName, unordered_map<int, sf::Texture> &numb,  int x, int y){
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            // Check if the current neighbor tile has a mine and is within the bounds of the board
            if (i >= 0 && j >= 0 && i < cols && j < rows && !tiles[i][j].show && !tiles[i][j].mine && !tiles[i][j].flags) {
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
    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles.at(i).size(); j++) {
            //checking if every tile that can be show has been show
            if (!tiles[i][j].mine && !tiles[i][j].show) {
                return false;
            }
        }
    }
    return true;
}

void ResetBoard(sf::Texture& original, int col, int row, int mine){
    cols = col;
    rows = row;
    mines = mine;

    for (int i = 0; i < cols; i++){
        for(int j = 0; j < rows; j++){
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
        // generate a random x and y position using Random.h
        int rand_x = Random::Int(0, cols - 1);
        int rand_y = Random::Int(0, rows - 1);

        // add Tile at randomized position if it is not already a mine
        if (!tiles[rand_x][rand_y].mine){
            tiles[rand_x][rand_y].SetMine();
            mineTotal++;
        }
    }

    for (int i = 0; i < cols; i++){         // determine all the neighbors for each of the individual tiles
        for (int j = 0; j < rows; j++){
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
void setText(sf::Text &text, float x, float y) {
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



    // Render the welcome screen

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
                    output.pop_back(); // Modify the existing name variable
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
            // Set cursor position to end of input text
            sf::FloatRect inputRect = text3.getLocalBounds();
            //cursor.setPosition(text3.getPosition().x + inputRect.width, text3.getPosition().y + inputRect.height);
            setText(cursor, (window.getSize().x) / 2 + inputRect.width * 0.6, ((window.getSize().y) / 2 - 45));
            window.draw(cursor);
        }
        window.display();

    }

    Board board(columnNew, rowNew, minesNew);



    // Render the game window



    sf::RenderWindow gameWindow(sf::VideoMode((columnNew)*32, ((rowNew)*32)+100), "Minesweeper",sf::Style::Close | sf::Style::Titlebar);

    // Load Counter Sprites
    int countVal1;
    int countVal2;
    int countVal3;
    int digit;
    if (minesNew >= 100){
        digit = 3;
        countVal1 = stoi(mines.substr(0, 1));
        countVal2 = stoi(mines.substr(1, 2));
        countVal3 = stoi(mines.substr(2, -1));
    }

    else if (minesNew >= 10 and minesNew <= 99){
        digit = 2;
        countVal1 = 0;
        countVal2 = stoi(mines.substr(0, 1));
        countVal3 = stoi(mines.substr(1, -1));
        //cout << countVal1 << endl;

    }
    else if (minesNew >= 0 and minesNew <= 9){
        digit = 1;
        countVal3 = stoi(mines.substr(0, -1));
        countVal2 = 0;
        countVal1 = 0;
    }
    // Render button sprites
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


    //Load Timer Sprites
    sf::Texture timeDigits;
    timeDigits.loadFromFile("files/images/digits.png", sf::IntRect(0, 0, 21, 32));
    Buttons minCol1(timeDigits, (columnNew) * 32 - 97, 32 * ((rowNew) + 0.5) + 16);
    Buttons minCol2(timeDigits, (columnNew) * 32 - 76, 32 * ((rowNew) + 0.5) + 16);
    Buttons secCol1(timeDigits, (columnNew) * 32 - 54, 32 * ((rowNew) + 0.5) + 16);
    Buttons secCol2(timeDigits, (columnNew) * 32 - 34, 32 * ((rowNew) + 0.5) + 16);



    // Load the leaderboard file
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
    //cout << name1;

    string name2 = line2.substr(line2.find(",") + 1, -1);
    string clock2 = line2.substr(0, line2.find(","));
    //cout << name2;
    string name3 = line3.substr(line3.find(",") + 1, -1);
    string clock3 = line3.substr(0, line3.find(","));
    // cout << name3;
    string name4 = line4.substr(line4.find(",") + 1, -1);
    string clock4 = line4.substr(0, line4.find(","));
    //cout << name4;
    string name5 = line5.substr(line5.find(",") + 1, -1);
    string clock5 = line5.substr(0, line5.find(","));
    //cout << name5;


    string final_name= output+"*";


    map<string, string> playerTimeName;
    playerTimeName.emplace(clock1, name1);
    playerTimeName.emplace(clock2, name2);
    playerTimeName.emplace(clock3, name3);
    playerTimeName.emplace(clock4, name4);
    playerTimeName.emplace(clock5, name5);





    // Create the timer
    auto start_time = chrono::high_resolution_clock::now();
    auto pause_time = chrono::high_resolution_clock::now();
    auto elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - pause_time).count();

    if (gameWindowOpen) {
        while (gameWindow.isOpen()) {
            gameWindow.clear(sf::Color::White);

            auto runTime = std::chrono::duration_cast<std::chrono::seconds>(chrono::high_resolution_clock::now() - start_time);
            int totalTime = runTime.count(); // necessary to subtract elapsed time later because "runTime.count()" is const

            if (board.CheckWin()) {
                win = true;
                hasWon = true;

            }

            int minute_left = minutes / 10 % 10; //minutes index 0
            int minute_right = minutes % 10; // minutes index 1
            int second_left = seconds / 10 % 10; // seconds index 0
            int second_right = seconds % 10; // seconds index 1

            string final_minute = to_string(minutes);
            string final_second = to_string(seconds);

            if (final_minute.length() < 2) {
                final_minute = "0" + final_minute;

            }
            if (final_second.length() < 2) {
                final_second = "0" + final_second;
            }
            // only update time if the game is not pause
            if (!pause_button) {
                totalTime = totalTime - elapsed_paused_time; //
                minutes = totalTime / 60;
                seconds = totalTime % 60;
            }


            // separate time value by index



            // create a string version of the time

            string final_time = final_minute+ ":" +final_second;





            for (int i = 0; i < board.cols; i++) {               // draw the board on the screen
                for (int j = 0; j < board.rows; j++) {
                    board.tiles[i][j].showSprite.setPosition(32 * i, 32 *j); // draw mine sprite on top of show sprite and make reveal mine return a sprite object
                    gameWindow.draw(board.tiles[i][j].showSprite);
                    auto currTile = board.tiles[i][j];


                    if (currTile.mine == true && currTile.show == true && !pause_button) {    // can be used for the debug
                        currTile.mineSprite.setPosition(32 * i, 32 * j);
                        gameWindow.draw(currTile.mineSprite);
                    }
                    else if (!currTile.mine && !pause_button && currTile.show ) {
                        currTile.numSprite.setPosition(32 * i, 32 * j);
                        gameWindow.draw(currTile.numSprite);
                    }
                    if (pause_button) {
                        sf::Sprite temp(textures["show"]);
                        temp.setPosition(currTile.showSprite.getPosition());
                        gameWindow.draw(temp);
                    }
                    if (debug_button && !lose && !win) {
                        currTile.mineSprite.setPosition(32 * i, 32 * j);
                        gameWindow.draw(currTile.mineSprite);
                    }
                    if (currTile.flags) {
                        currTile.flagSprite.setPosition(32 * i, 32 * j);
                        gameWindow.draw(currTile.flagSprite);
                    }


                    if (win) {

                        happy.Set_Texture(textures["win"]); // change face



                        //place flags on all the mines
                        if (currTile.mine && !currTile.flags) {
                            currTile.SetFlag();
                            currTile.flagSprite.setPosition(32 * i, 32 * j);
                            gameWindow.draw(currTile.flagSprite);

                        }


                        if (around && hasWon) {
                            hasWon = false;
                            around = false;
                            playerTimeName.emplace(final_time, final_name);

                            // If the player has won we need to write to the leaderboard textfile

                            ofstream leader_file("files/leaderboard.txt", ios_base::out); // output stream file

                            for (auto iter = playerTimeName.begin(); iter != playerTimeName.end(); iter++) {

                                if (iter->second.find("*") != string::npos) {
                                    playerTimeName[iter->first] = iter->second.substr(0, iter->second.size());

                                }
                                if (playerTimeName.size() > 5) {
                                    playerTimeName.erase(--playerTimeName.end());
                                }





                                leader_file.write((iter->first + "," + iter->second + "\n").c_str(),(iter->first + "," + iter->second + "\n").length());


                            }

                            sf::RenderWindow leaderboard(sf::VideoMode(16 * columnNew, (16 * rowNew) + 50),"Leaderboard", sf::Style::Close | sf::Style::Titlebar);

                            sf::Text text4((printLeaderBoard(playerTimeName)),font,18);
                            text4.setStyle(sf::Text::Bold);
                            text4.setFillColor(sf::Color::White);
                            setText(text4, 16 * columnNew / 2.0, 17 * rowNew / 2.0+ 20);

                            sf::Text text5("LEADERBOARD",font,20);
                            text5.setFillColor(sf::Color::White);
                            text5.setStyle(sf::Text::Underlined | sf::Text::Bold);
                            setText(text5, 16*columnNew / 2.0,  17*rowNew / 2.0 - 120);




                            // while the leaderboard is open
                            while (leaderboard.isOpen()) {

                                sf::Event event2;
                                while (leaderboard.pollEvent(event2)) {
                                    if (event2.type == sf::Event::Closed) {
                                        hasWon = false;
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
                        happy.Set_Texture(textures["sad"]);
                        if (currTile.mine && !currTile.flags) {
                            currTile.showSprite.setTexture(textures["show"]);
                            currTile.mineSprite.setPosition(32 * i, 32 * j);
                            currTile.showSprite.setPosition(32 * i, 32 * j);
                            gameWindow.draw(currTile.showSprite);
                            gameWindow.draw(currTile.mineSprite);

                        }

                    }

                    if (reset) {

                        around = true;
                        reset = false;

                        if (win) {
                            win = false;
                            happy.Set_Texture(textures["face_happy"]);
                        }
                        else {
                            lose = false;
                            happy.Set_Texture(textures["face_happy"]);
                        }
                    }


                }
            }

            sf::Event event3;

            while (gameWindow.pollEvent(event3)) {
                if (event3.type == sf::Event::Closed) {
                    gameWindow.close();
                }

                if (event3.type == sf::Event::MouseButtonPressed) {

                    sf::Mouse mouse;
                    auto pos = mouse.getPosition(gameWindow);     // pos of click

                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {   // if it is a left click
                        //If restart button is clicked
                        auto Happy_Rect = happy.open().getGlobalBounds();
                        if (Happy_Rect.contains(pos.x, pos.y)) {
                            reset = true;
                            pause_button = false;

                            happy.Set_Texture(textures["face_happy"]);
                            board.ResetBoard(textures["unrevealed"], columnNew, rowNew, minesNew);

                            flag_count = 0;
                            flags_placed= 0;
                            showCount = 0;

                            // Reset timer
                            start_time = chrono::high_resolution_clock::now();

                            // Reset counter
                            nega_tive = false;
                            count1.Set_Texture(time[countVal1]);
                            count2.Set_Texture(time[countVal2]);
                            count3.Set_Texture(time[countVal3]);
                        }
                        // If leaderboard button is clicked
                        auto Leaderboard_Rect = leaderboardButton.open().getGlobalBounds();
                        if (Leaderboard_Rect.contains(pos.x, pos.y)) {
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
                            if (leader) {
                                pause_time = chrono::high_resolution_clock::now();

                                // render the leaderboard window
                                sf::RenderWindow leaderboard(sf::VideoMode(16 * columnNew, (16 * rowNew) + 50),"Leaderboard", sf::Style::Close | sf::Style::Titlebar);

                                sf::Text text4((printLeaderBoard(playerTimeName)),font,18);
                                text4.setStyle(sf::Text::Bold);
                                text4.setFillColor(sf::Color::White);
                                setText(text4, 16 * columnNew / 2.0, 17 * rowNew / 2.0+ 20);

                                sf::Text text5("LEADERBOARD",font,20);
                                text5.setFillColor(sf::Color::White);
                                text5.setStyle(sf::Text::Underlined | sf::Text::Bold);
                                setText(text5, 16*columnNew / 2.0,  17*rowNew / 2.0 - 120);

                                // while the leaderboard is open
                                while (leaderboard.isOpen()) {
                                    sf::Event event2;
                                    while (leaderboard.pollEvent(event2)) {
                                        if (event2.type == sf::Event::Closed) {
                                            leader = false;
                                            leaderboard.close();
                                            auto resumeTime = chrono::high_resolution_clock::now();   // calculate amount of time that leaderboard was open
                                            elapsed_paused_time = elapsed_paused_time += chrono::duration_cast<chrono::seconds>(
                                                    resumeTime - pause_time).count();

                                        }
                                    }
                                    //gameWindow.display();
                                    leaderboard.clear(sf::Color::Blue);
                                    leaderboard.draw(text5);
                                    leaderboard.draw(text4);
                                    leaderboard.display();

                                }

                            }
                        }


                        if (!lose and !win) {
                            // if Pause button is clicked
                            auto Pause_Rect = pause.open().getGlobalBounds();
                            if (Pause_Rect.contains(pos.x, pos.y)) {
                                pause_button = !pause_button;
                                if (pause_button) {
                                    pause.Set_Texture(textures["play"]);
                                    // Pause Timer
                                    pause_time = chrono::high_resolution_clock::now();

                                }
                                else {
                                    pause.Set_Texture(textures["pause"]);
                                    auto resumeTime = chrono::high_resolution_clock::now();   // calculate amount of time that it was pause;
                                    elapsed_paused_time = elapsed_paused_time += chrono::duration_cast<chrono::seconds>(resumeTime - pause_time).count();

                                }

                            }


                            if (!pause_button) {          // checks if gameWindow is pause
                                for (int i = 0; i < board.cols; i++) {
                                    for (int j = 0; j < board.rows; j++) {
                                        auto Tile_rect = board.tiles[i][j].showSprite.getGlobalBounds();         // gets global bounds of an individual tile
                                        if (Tile_rect.contains(pos.x, pos.y)) {
                                            if (board.tiles[i][j].mine and
                                                !board.tiles[i][j].flags) {            // checks if a tile is a mine and that if it is flags
                                                board.tiles[i][j].SetShow(textures["show"]);              // reveals the tile
                                                lose = true;

                                            }
                                            else if (!board.tiles[i][j].flags) {                     // checks if a tile is flags
                                                if (board.tiles[i][j].neighbor_mines != 0) {
                                                    board.tiles[i][j].SetShow(textures["show"]);
                                                    board.tiles[i][j].SetNumber(number[board.tiles[i][j].neighbor_mines]);
                                                }
                                                else if (board.tiles[i][j].neighbor_mines == 0) {
                                                    board.showArea(textures["show"], number, i, j);
                                                }

                                            }
                                        }
                                    }
                                }

                                // if debug button is clicked
                                auto Debug_Rect = debug.open().getGlobalBounds();
                                if (Debug_Rect.contains(pos.x, pos.y)) {
                                    debug_button = !debug_button;           // flips the debug boolean value
                                }

                            }
                        }
                    } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                        if (!win and !lose) {
                            for (int i = 0; i < board.cols; i++) {
                                for (int j = 0; j < board.rows; j++) {
                                    auto Flag_rect = board.tiles[i][j].showSprite.getGlobalBounds();
                                    if (Flag_rect.contains(pos.x, pos.y) and
                                        !board.tiles[i][j].show) {
                                        if (board.tiles[i][j].flags) {
                                            flags_placed--;
                                        }
                                        else {
                                            flags_placed++;
                                        }

                                        board.tiles[i][j].SetFlag();
                                        counterVal = minesNew - flags_placed;


                                        if (counterVal < 0) {
                                            nega_tive = true;
                                            newVal1 = -1 * (counterVal / 100);
                                            newVal2 = -1 * (counterVal / 10);
                                            newVal3 = -1 * (counterVal % 10);
                                        }
                                        else {
                                            nega_tive = false;
                                            if (digit == 1) {
                                                newVal3 = minesNew - flags_placed;
                                            }
                                            else if (digit == 2) {
                                                newVal2 = counterVal / 10;
                                                newVal3 = counterVal % 10;

                                            }
                                            else if (digit == 3) {
                                                newVal1 = counterVal / 100;
                                                newVal2 = (counterVal % 100) / 10;
                                                newVal3 = counterVal % 10;
                                            }

                                        }
                                        // Change the counter texture
                                        count1.Set_Texture(time[newVal1]);
                                        count2.Set_Texture(time[newVal2]);
                                        count3.Set_Texture(time[newVal3]);
                                    }
                                }
                            }
                        }
                    }
                }
            }


            if (!win and !lose) {

                minCol1.Set_Texture(time[minute_left]);
                minCol2.Set_Texture(time[minute_right]);
                secCol1.Set_Texture(time[second_left]);
                secCol2.Set_Texture(time[second_right]);

            }

            gameWindow.draw(secCol2.open());
            gameWindow.draw(secCol1.open());
            gameWindow.draw(minCol2.open());
            gameWindow.draw(minCol1.open());


            if (nega_tive) {
                gameWindow.draw(negative.open());
            }
            gameWindow.draw(count1.open());
            gameWindow.draw(count2.open());
            gameWindow.draw(count3.open());
            gameWindow.draw(happy.open());
            gameWindow.draw(leaderboardButton.open());
            gameWindow.draw(debug.open());
            gameWindow.draw(pause.open());



            gameWindow.display();

        }

    }
};



all in one while loop

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cctype>
#include <cmath>
using namespace std;

void setText(sf::Text &text, float x, float y) {
sf::FloatRect textRect = text.getLocalBounds();
text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
text.setPosition(sf::Vector2f(x, y));
}

int main() {
sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window", sf::Style::Close);
sf::Font font;
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
    setText(text3, (window.getSize().x) / 2, ((window.getSize().y) / 2-45));
    text3.setFillColor(sf::Color::Yellow);

    sf::Text cursor("|", font, 20);
    cursor.setFillColor(sf::Color::Yellow);
    cursor.setStyle(sf::Text::Bold);

    bool cursorVisible = true;
    sf::Clock cursorClock;

    bool gameWindowOpen = false; // Flag to track if the game window is open

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered) {
                if (isalpha(event.text.unicode) && text3.getString().getSize() < 10) {
                    std::string inputString = text3.getString().toAnsiString();
                    if (inputString.empty()) {
                        inputString += std::toupper(static_cast<char>(event.text.unicode));
                    } else {
                        inputString += std::tolower(static_cast<char>(event.text.unicode));
                    }
                    text3.setString(inputString);
                    setText(text3, (window.getSize().x) / 2, ((window.getSize().y) / 2-45));

                } else if (event.text.unicode == '\b' && !text3.getString().isEmpty()) {
                    string inputString = text3.getString();
                    inputString.pop_back();
                    text3.setString(inputString);
                    setText(text3, (window.getSize().x) / 2, ((window.getSize().y) / 2-45));

                } else if (event.text.unicode == '\r' && !text3.getString().isEmpty()) {
                    // Close the input window and open the game window if at least one character is input
                    window.close();
                    gameWindowOpen = true;
                }
            }
        }

        // Open the game window if requested
        if (gameWindowOpen) {
            sf::RenderWindow gameWindow(sf::VideoMode(800, 600), "Game Window", sf::Style::Close);

            while (gameWindow.isOpen()) {
                sf::Event event;
                while (gameWindow.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        gameWindow.close();
                    }
                }

                gameWindow.clear(sf::Color::White);

                // Render your game content here

                gameWindow.display();
            }

            // Reset the flag after the game window is closed
            gameWindowOpen = false;
        }


        window.clear(sf::Color::Blue);

        window.draw(text);
        window.draw(text2);
        window.draw(text3);

        // Cursor blinking
        if (cursorClock.getElapsedTime().asSeconds() > 0.5) {
            cursorVisible = !cursorVisible;
            cursorClock.restart();
        }
        if (cursorVisible) {
            // Set cursor position to end of input text
            sf::FloatRect inputRect = text3.getLocalBounds();
            cursor.setPosition(text3.getPosition().x +inputRect.width, text3.getPosition().y+inputRect.height);
            setText(cursor, (window.getSize().x) / 2+ inputRect.width*0.6, ((window.getSize().y) / 2-45));
            window.draw(cursor);
        }

        window.display();
    }

    return 0;
}







seperated while loop


#include <SFML/Graphics.hpp>
#include <iostream>
#include <cctype>
#include <cmath>
using namespace std;

void setText(sf::Text &text, float x, float y) {
sf::FloatRect textRect = text.getLocalBounds();
text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
text.setPosition(sf::Vector2f(x, y));
}

int main() {
sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window", sf::Style::Close);
sf::Font font;

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
    setText(text3, (window.getSize().x) / 2, ((window.getSize().y) / 2-45));
    text3.setFillColor(sf::Color::Yellow);

    sf::Text cursor("|", font, 20);
    cursor.setFillColor(sf::Color::Yellow);
    cursor.setStyle(sf::Text::Bold);

    bool cursorVisible = true;
    sf::Clock cursorClock;

    bool gameWindowOpen = false; // Flag to track if the game window is open




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
                    std::string inputString = text3.getString().toAnsiString();
                    if (inputString.empty()) {
                        inputString += std::toupper(static_cast<char>(event.text.unicode));
                    } else {
                        inputString += std::tolower(static_cast<char>(event.text.unicode));
                    }
                    text3.setString(inputString);
                    setText(text3, (window.getSize().x) / 2, ((window.getSize().y) / 2-45));

                } else if (event.text.unicode == '\b' && !text3.getString().isEmpty()) {
                    string inputString = text3.getString();
                    inputString.pop_back();
                    text3.setString(inputString);
                    setText(text3, (window.getSize().x) / 2, ((window.getSize().y) / 2-45));

                } else if (event.text.unicode == '\r' && !text3.getString().isEmpty()) {
                    // Close the input window and open the game window if at least one character is input
                    //window.display();
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
            // Set cursor position to end of input text
            sf::FloatRect inputRect = text3.getLocalBounds();
            cursor.setPosition(text3.getPosition().x +inputRect.width, text3.getPosition().y+inputRect.height);
            setText(cursor, (window.getSize().x) / 2+ inputRect.width*0.6, ((window.getSize().y) / 2-45));
            window.draw(cursor);
        }
        window.display();

    }


        // Open the game window if requested
    if (gameWindowOpen) {
        sf::RenderWindow gameWindow(sf::VideoMode(800, 600), "Game Window", sf::Style::Close);


        while (gameWindow.isOpen()) {
            sf::Event event;
            gameWindow.clear(sf::Color::White);

            while (gameWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    gameWindow.close();
                }
            }



            gameWindow.display();
        }

        // Reset the flag after the game window is closed
        //gameWindowOpen = false;

    }
    return 0;
}





#include <SFML/Graphics.hpp>
#include <iostream>
#include <cctype>
#include <cmath>
#include <fstream>
#include "TextureNew.h"
#include "RandomNew.h"
#include <string>
#include <random>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <atomic>
#include <cstring>
#include <sstream>
#include <iomanip>
#include "ButtonNew.h"

using namespace std;

class Tile{
public:

    sf::Vector2f position;   // used to set the position of the tile
    sf::Sprite reveal_sprite;          // the sprite that is used to represent the tile
    sf::Texture texture;        // string for the texture of the sprite

    sf::Texture mine_texture;
    sf::Sprite mine_sprite;

    sf::Texture flag_texture;
    sf::Sprite flag_sprite;

    sf::Sprite num_sprite;


    bool flagged;
    bool mine;
    bool revealed;
    bool num;

    vector<Tile*> neighbors;  // adjacent tiles

    int neighbor_bombs;
    int value;  // 1 for mine 0 for not a mine




    Tile(sf::Vector2f _position, string _texture){
        texture.loadFromFile(_texture);
        reveal_sprite = sf::Sprite(texture);
        reveal_sprite.setPosition(_position.x, _position.y);
        position = _position;

        flagged = false;
        mine = false;
        revealed = false;


    }

    Tile(){
        texture.loadFromFile("files/images/tile_hidden.png"); // default texture and sprite are set
        reveal_sprite = sf::Sprite(texture);


        flagged = false;
        mine = false;
        revealed = false;
        num = false;
        neighbor_bombs = 0;
        value = 0;


    }

    void SetMine(){
        mine = true;
        value = 1;

        mine_texture.loadFromFile("files/images/mine.png");
        mine_sprite = sf::Sprite(mine_texture);

    }

    void SetFlag(){
        flagged = !flagged;
        flag_texture.loadFromFile("files/images/flag.png");
        flag_sprite = sf::Sprite(flag_texture);

    }

    void SetReveal(sf::Texture& _texture){                  // we need to recursive all the function on itself and reveal a sprite using the
        // same loop type as we did for the adjacent mines
        revealed = true;
        reveal_sprite = sf::Sprite(_texture);

    }

    void SetSprite(string _texture){
        texture.loadFromFile(_texture);         // set the texture of the sprite
        reveal_sprite = sf::Sprite(texture);
    }

    void SetPosition(sf::Vector2f _position){
        reveal_sprite.setPosition(_position.x, _position.y);       // set the position of the sprite
    }

    void SetNumber(sf::Texture& _texture){
        num = true;
        num_sprite = sf::Sprite(_texture);
    }

    void SetUnrevealed(sf::Texture& _texture){
        revealed = false;
        reveal_sprite = sf::Sprite(_texture);
    }
};

class Board{
public:
    int cols;
    int rows;
    int mines;
    vector<vector<Tile>> tiles;
    int numRevealedTiles;
    bool gameWon;
    bool gameOver;


    Board(int _cols, int _rows, int _mines){
        cols = _cols;
        rows = _rows;
        mines = _mines;

        tiles.resize(cols);         // fill vector with tiles which by default are not mines

        for(int i = 0; i < cols; i++){
            tiles[i].resize(rows);
        }

        // place mines in places according to random positions
        int mine_count = 0;

        while (mine_count < mines){
            // generate a random x and y position using Random.h
            int rand_x = Random::Int(0, cols - 1);
            int rand_y = Random::Int(0, rows - 1);

            // add Tile at randomized position if it is not already a mine
            if (!tiles[rand_x][rand_y].mine){
                tiles[rand_x][rand_y].SetMine();
                mine_count++;
            }
        }

        for (int i = 0; i < cols; i++){         // determine all the neighbors for each of the individual tiles
            for (int j = 0; j < rows; j++){
                determine_neighbor_mines(i, j);
            }
        }



        //cout << "END" << endl;
    }

    void determine_neighbors(sf::Texture _reveal,sf::Texture _number,  int x, int y){     // pass in the position of the mine you want to check
        int count = 0;

        // Iterate over neighboring tiles in a 3x3 grid around the current tile
        for (int i = x - 1; i <= x + 1; i++) {
            for (int j = y - 1; j <= y + 1; j++) {
                // Check if the current neighbor tile has a mine and is within the bounds of the board
                if (i >= 0 && i < cols && j >= 0 && j < rows && !tiles[i][j].mine and !tiles[i][j].flagged and !tiles[i][j].revealed) {
                    if (tiles[i][j].num) {
                        tiles[i][j].SetReveal(_reveal);
                        tiles[i][j].SetNumber(_number);
                    }
                    else{
                        tiles[i][j].SetReveal(_reveal);
                        determine_neighbors(_reveal, _number, i, j);
                    }

                }
            }
        }

    }

    void determine_neighbor_mines(int x, int y){
        int count = 0;

        // Iterate over neighboring tiles in a 3x3 grid around the current tile
        for (int i = x - 1; i <= x + 1; i++) {
            for (int j = y - 1; j <= y + 1; j++) {
                // Check if the current neighbor tile has a mine and is within the bounds of the board
                if (i >= 0 && i < cols && j >= 0 && j < rows && tiles[i][j].mine) {
                    count++;
                }
            }
        }

        tiles[x][y].neighbor_bombs = count;
    }

    void Print_TestBoard(){
        for (int i = 0; i < cols; i++){         // determine all the neighbors for each of the individual tiles
            for (int j = 0; j < rows; j++){
                cout << tiles[i][j].value;
            }
            cout << endl;
        }
    }

    void Reveal_around(sf::Texture &_texture, unordered_map<int, sf::Texture> &_numbers,  int x, int y){
        for (int i = x - 1; i <= x + 1; i++) {
            for (int j = y - 1; j <= y + 1; j++) {
                // Check if the current neighbor tile has a mine and is within the bounds of the board
                if (i >= 0 && i < cols && j >= 0 && j < rows && !tiles[i][j].mine and !tiles[i][j].flagged and !tiles[i][j].revealed) {
                    tiles[i][j].SetReveal(_texture);
                    if (tiles[i][j].neighbor_bombs != 0){
                        tiles[i][j].SetNumber(_numbers[tiles[i][j].neighbor_bombs]);
                    }
                    else{
                        Reveal_around(_texture, _numbers, i, j);
                    }
                }
            }
        }
    }

    void ResetBoard(sf::Texture& _default, int _cols, int _rows, int _mines){
        cols = _cols;
        rows = _rows;
        mines = _mines;

        for (int i = 0; i < cols; i++){
            for(int j = 0; j < rows; j++){
                tiles[i][j].mine = false;
                tiles[i][j].flagged = false;
                tiles[i][j].revealed = false;
                tiles[i][j].num = false;
                tiles[i][j].neighbor_bombs = 0;
                tiles[i][j].reveal_sprite.setTexture(_default);
                tiles[i][j].mine_sprite.setTexture(_default);
                tiles[i][j].num_sprite.setColor(sf::Color::Transparent);
            }
        }

        int mine_count = 0;

        while (mine_count < mines){
            // generate a random x and y position using Random.h
            int rand_x = Random::Int(0, cols - 1);
            int rand_y = Random::Int(0, rows - 1);

            // add Tile at randomized position if it is not already a mine
            if (!tiles[rand_x][rand_y].mine){
                tiles[rand_x][rand_y].SetMine();
                mine_count++;
            }
        }

        for (int i = 0; i < cols; i++){         // determine all the neighbors for each of the individual tiles
            for (int j = 0; j < rows; j++){
                determine_neighbor_mines(i, j);
            }
        }

    }

    bool CheckWin() {
        for (int i = 0; i < tiles.size(); i++) {
            for (int j = 0; j < tiles.at(i).size(); j++) {
                //checking if every tile that can be revealed has been revealed
                if (!tiles[i][j].revealed && !tiles[i][j].mine) {
                    return false;
                }
            }
        }
        return true;
    }
};




void PrintMine(sf::RenderWindow &window, float x_cord, float y_cord){
    sf::Texture mine_texture;
    mine_texture.loadFromFile("files/images/mine.png");
    sf::Sprite mine(mine_texture);
    mine.setPosition(x_cord, y_cord);
    window.draw(mine);

}

string capitalize_first_letter(string name) {
    if (name.empty()) {
        return name;
    } else {
        name[0] = toupper(name[0]);
        for (std::size_t i = 1; i < name.length(); i++) {
            name[i] = tolower(name[i]);
        }
        return name;
    }
}

string print_leaderboard(map<string, string> &_players){
    string output = "";
    int index = 1;
    auto iter = _players.begin();
    for (auto iter = _players.begin(); iter != _players.end(); iter++){
        output.append(to_string(index) + ". \t" + iter->first + "\t" + iter->second + "\n\n");
        index++;
    }

    return output;
}



void setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

int main() {
    sf::Font font;
    bool firsttimearound = true;
    unordered_map<string, sf::Texture> textures;

    textures["mine"].loadFromFile("files/images/mine.png");
    textures["unrevealed"].loadFromFile("files/images/tile_hidden.png");
    textures["revealed"].loadFromFile("files/images/tile_revealed.png");
    textures["flag"].loadFromFile("files/images/flag.png");
    textures["play"].loadFromFile("files/images/play.png");
    textures["pause"].loadFromFile("files/images/pause.png");
    textures["win"].loadFromFile("files/images/face_win.png");
    textures["sad"].loadFromFile("files/images/face_lose.png");
    textures["happy_face"].loadFromFile("files/images/face_happy.png");

    unordered_map<int, sf::Texture> numbers;
    numbers[1].loadFromFile("files/images/number_1.png");
    numbers[2].loadFromFile("files/images/number_2.png");
    numbers[3].loadFromFile("files/images/number_3.png");
    numbers[4].loadFromFile("files/images/number_4.png");
    numbers[5].loadFromFile("files/images/number_5.png");
    numbers[6].loadFromFile("files/images/number_6.png");
    numbers[7].loadFromFile("files/images/number_7.png");
    numbers[8].loadFromFile("files/images/number_8.png");


    unordered_map<int, sf::Texture> clock;
    clock[0].loadFromFile("files/images/digits.png", sf::IntRect(0, 0, 21, 32));
    clock[1].loadFromFile("files/images/digits.png", sf::IntRect(21, 0, 21, 32));
    clock[2].loadFromFile("files/images/digits.png", sf::IntRect(42, 0, 21, 32));
    clock[3].loadFromFile("files/images/digits.png", sf::IntRect(63, 0, 21, 32));
    clock[4].loadFromFile("files/images/digits.png", sf::IntRect(84, 0, 21, 32));
    clock[5].loadFromFile("files/images/digits.png", sf::IntRect(105, 0, 21, 32));
    clock[6].loadFromFile("files/images/digits.png", sf::IntRect(126, 0, 21, 32));
    clock[7].loadFromFile("files/images/digits.png", sf::IntRect(147, 0, 21, 32));
    clock[8].loadFromFile("files/images/digits.png", sf::IntRect(168, 0, 21, 32));
    clock[9].loadFromFile("files/images/digits.png", sf::IntRect(189, 0, 21, 32));
    clock[10].loadFromFile("files/images/digits.png", sf::IntRect(210, 0, 21, 32));

    string cols;
    string rows;
    string mines;
    ifstream file;
    file.open("files/config.cfg");
    if (!file.is_open()) {
        cout << "Error" << endl;
    } else {
        getline(file, cols);
        getline(file, rows);
        getline(file, mines);

    }
    int config_cols = stoi(cols);
    int config_rows = stoi(rows);
    int config_mines = stoi(mines);

    Board board(config_cols, config_rows, config_mines);

    sf::RenderWindow window(sf::VideoMode((stoi(cols)
    *32)+100, stoi(rows) * 32), "Welcome Window", sf::Style::Close);


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

    bool cursorVisible = true;
    sf::Clock cursorClock;

    bool gameWindowOpen = false; // Flag to track if the game window is open



    // Render button sprites
    sf::Texture happy_face;
    happy_face.loadFromFile("files/images/face_happy.png");
    Buttons happy(happy_face, (((config_cols) / 2.0) * 32) - 32, 32 * (config_rows + 0.5f));

    sf::Texture debug_texture;
    debug_texture.loadFromFile("files/images/debug.png");
    Buttons debug(debug_texture, (((config_cols) * 32) - 304), 32 * (config_rows + 0.5f));

    sf::Texture pause_button;
    pause_button.loadFromFile("files/images/pause.png");
    Buttons pause(pause_button, (((config_cols) * 32) - 240), 32 * (config_rows + 0.5f));

    sf::Texture leader_texture;
    leader_texture.loadFromFile("files/images/leaderboard.png");
    Buttons leader_button(leader_texture, (((config_cols) * 32) - 176), 32 * (config_rows + 0.5f));

    //Load Timer Sprites
    sf::Texture default_time;
    default_time.loadFromFile("files/images/digits.png", sf::IntRect(0, 0, 21, 32));
    Buttons minute_col1(default_time, (config_cols) * 32 - 97, 32 * ((config_rows) + 0.5f) + 16);
    Buttons minute_col2(default_time, (config_cols) * 32 - 97 + 21, 32 * ((config_rows) + 0.5f) + 16);
    Buttons sec_col1(default_time, (config_cols) * 32 - 54, 32 * ((config_rows) + 0.5f) + 16);
    Buttons sec_col2(default_time, (config_cols) * 32 - 54 + 21, 32 * ((config_rows) + 0.5f) + 16);

    // Load Counter Sprites
    int count_val0;
    int count_val1;
    int count_val2;
    int digi_num;
    if (config_mines >= 0 and config_mines < 10) {
        digi_num = 1;
        count_val2 = stoi(mines.substr(0, -1));
        count_val1 = 0;
        count_val0 = 0;
    } else if (config_mines >= 10 and config_mines < 100) {
        digi_num = 2;
        count_val0 = 0;
        count_val1 = stoi(mines.substr(0, 1));
        count_val2 = stoi(mines.substr(1, -1));
        //cout << count_val1 << endl;

    } else if (config_mines >= 100) {
        digi_num = 3;
        count_val0 = stoi(mines.substr(0, 1));
        count_val1 = stoi(mines.substr(1, 2));
        count_val2 = stoi(mines.substr(2, -1));


    }

    Buttons count0(clock[count_val0], 33, 32 * ((config_rows) + 0.5f) + 16);
    Buttons count1(clock[count_val1], 54, 32 * ((config_rows) + 0.5f) + 16);
    Buttons count2(clock[count_val2], 75, 32 * ((config_rows) + 0.5f) + 16);
    Buttons negative(clock[10], 12, 32 * ((config_rows) + 0.5f) + 16);

    sf::RenderWindow gameWindow(sf::VideoMode((stoi(cols)*32)+100, stoi(rows)*32), "Game Window", sf::Style::Close);




    //
    bool debug_pushed = false;
    bool win = false;
    bool lost = false;
    bool paused = false;
    bool reset = false;
    bool leader = false;
    int flag_count = 0;
    int flags_placed = 0;
    int reveal_count = 0;
    int counter_val;
    int new_val0;
    int new_val1;
    int new_val2;
    bool neg;
    bool render;
    int minutes;
    int seconds;
    bool hasWon = false;
    string input_name;
    string final_name;
    final_name = capitalize_first_letter(input_name) + "*";
    // Load the leaderboard file
    ifstream leader_file("files/leaderboard.txt");
    string line1;
    string line2;
    string line3;
    string line4;
    string line5;
    getline(leader_file, line1);
    getline(leader_file, line2);
    getline(leader_file, line3);
    getline(leader_file, line4);
    getline(leader_file, line5);

    string name1 = line1.substr(line1.find(",") + 1, -1);
    string time1 = line1.substr(0, line1.find(","));
    //cout << name1;

    string name2 = line2.substr(line2.find(",") + 1, -1);
    string time2 = line2.substr(0, line2.find(","));
    //cout << name2;
    string name3 = line3.substr(line3.find(",") + 1, -1);
    string time3 = line3.substr(0, line3.find(","));
    // cout << name3;
    string name4 = line4.substr(line4.find(",") + 1, -1);
    string time4 = line4.substr(0, line4.find(","));
    //cout << name4;
    string name5 = line5.substr(line5.find(",") + 1, -1);
    string time5 = line5.substr(0, line5.find(","));
    //cout << name5;
    map<string, string> players;
    players.emplace(time1, name1);
    players.emplace(time2, name2);
    players.emplace(time3, name3);
    players.emplace(time4, name4);
    players.emplace(time5, name5);





    // Create the timer
    auto start_time = chrono::high_resolution_clock::now();
    auto pauseTime = chrono::high_resolution_clock::now();
    auto elapsed_paused_time = chrono::duration_cast<chrono::seconds>(
            chrono::high_resolution_clock::now() - pauseTime).count();


    while (window.isOpen()) {
        sf::Event event;
        window.clear(sf::Color::Blue);

        window.draw(text);
        window.draw(text2);
        window.draw(text3);

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered) {
                if (isalpha(event.text.unicode) && text3.getString().getSize() < 10) {
                    string inputString = text3.getString().toAnsiString();
                    if (inputString.empty()) {
                        inputString += std::toupper(static_cast<char>(event.text.unicode));
                    } else {
                        inputString += std::tolower(static_cast<char>(event.text.unicode));
                    }
                    text3.setString(inputString);
                    setText(text3, (window.getSize().x) / 2, ((window.getSize().y) / 2 - 45));

                } else if (event.text.unicode == '\b' && !text3.getString().isEmpty()) {
                    string inputString = text3.getString();
                    inputString.pop_back();
                    text3.setString(inputString);
                    setText(text3, (window.getSize().x) / 2, ((window.getSize().y) / 2 - 45));

                } else if (event.text.unicode == '\r' && !text3.getString().isEmpty()) {
                    // Close the input window and open the game window if at least one character is input
                    //window.display();
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
            // Set cursor position to end of input text
            sf::FloatRect inputRect = text3.getLocalBounds();
            cursor.setPosition(text3.getPosition().x + inputRect.width, text3.getPosition().y + inputRect.height);
            setText(cursor, (window.getSize().x) / 2 + inputRect.width * 0.6, ((window.getSize().y) / 2 - 45));
            window.draw(cursor);
        }
        window.display();

    }
    if (gameWindowOpen) {
        while (gameWindow.isOpen()) {
            auto game_runtime = std::chrono::duration_cast<std::chrono::seconds>(
                    chrono::high_resolution_clock::now() - start_time);
            int total_time = game_runtime.count(); // necessary to subtract elapsed time later because "game_runtime.count()" is const
            gameWindow.clear(sf::Color::White);


            // only update time if the game is not paused
            if (!paused) {
                total_time = total_time - elapsed_paused_time; //
                minutes = total_time / 60;
                seconds = total_time % 60;
            }


            // separate time value by index
            int minutes0 = minutes / 10 % 10; //minutes index 0
            int minutes1 = minutes % 10; // minutes index 1
            int seconds0 = seconds / 10 % 10; // seconds index 0
            int seconds1 = seconds % 10; // seconds index 1


            // create a string version of the time
            string minutes_final = to_string(minutes);
            string seconds_final = to_string(seconds);

            if (minutes_final.length() < 2) {
                minutes_final = "0" + minutes_final;

            }
            if (seconds_final.length() < 2) {
                seconds_final = "0" + seconds_final;
            }

            string final_time = minutes_final + ":" + seconds_final;




            if (board.CheckWin()) {
                win = true;
                hasWon = true;

            }

            for (int i = 0; i < board.cols; i++) {               // draw the board on the screen
                for (int j = 0; j < board.rows; j++) {
                    board.tiles[i][j].reveal_sprite.setPosition(32 * i, 32 *
                                                                        j); // draw mine sprite on top of revealed sprite and make reveal mine return a sprite object
                    gameWindow.draw(board.tiles[i][j].reveal_sprite);
                    auto currentTile = board.tiles[i][j];

                    if (paused) {
                        sf::Sprite temp(textures["revealed"]);
                        temp.setPosition(currentTile.reveal_sprite.getPosition());
                        gameWindow.draw(temp);
                    }

                    if (currentTile.mine == true and currentTile.revealed == true and
                        !paused) {    // can be used for the debug
                        currentTile.mine_sprite.setPosition(32 * i, 32 * j);
                        gameWindow.draw(currentTile.mine_sprite);
                    } else if (!currentTile.mine and currentTile.revealed and !paused) {
                        currentTile.num_sprite.setPosition(32 * i, 32 * j);
                        gameWindow.draw(currentTile.num_sprite);
                    }
                    if (currentTile.flagged) {
                        currentTile.flag_sprite.setPosition(32 * i, 32 * j);
                        gameWindow.draw(currentTile.flag_sprite);
                    }
                    if (debug_pushed and !win and !lost) {
                        currentTile.mine_sprite.setPosition(32 * i, 32 * j);
                        gameWindow.draw(currentTile.mine_sprite);
                    }

                    if (win) {

                        happy.Set_Texture(textures["win"]); // change face

                        // reset counter
                        //count0.Set_Texture(clock[0]);
                        //count1.Set_Texture(clock[0]);
                        //count2.Set_Texture(clock[0]);

                        //place flags on all the mines
                        if (currentTile.mine and !currentTile.flagged) {
                            currentTile.SetFlag();
                            currentTile.flag_sprite.setPosition(32 * i, 32 * j);
                            gameWindow.draw(currentTile.flag_sprite);

                        }


                        if (hasWon and firsttimearound) {
                            hasWon = false;
                            firsttimearound = false;
                            players.emplace(final_time, final_name);

                            // If the player has won we need to write to the leaderboard textfile

                            ofstream leader_file("files/leaderboard.txt", ios_base::out); // output stream file

                            for (auto iter = players.begin(); iter != players.end(); iter++) {
                                if (iter->second.find("*") != string::npos) {
                                    players[iter->first] = iter->second.substr(0, iter->second.size());

                                }
                                if (players.size() > 5) {
                                    players.erase(--players.end());
                                }

                                leader_file.write((iter->first + "," + iter->second + "\n").c_str(),
                                                  (iter->first + "," + iter->second + "\n").length());


                            }

                            sf::RenderWindow leaderboard(sf::VideoMode(16 * config_cols, (16 * config_rows) + 50),
                                                         "Leaderboard", sf::Style::Close | sf::Style::Titlebar);
                            sf::Text Leader_title;
                            Leader_title.setFont(font);
                            Leader_title.setCharacterSize(20);
                            Leader_title.setFillColor(sf::Color::White);
                            Leader_title.setString("LEADERBOARD");
                            Leader_title.setStyle(sf::Text::Bold | sf::Text::Underlined);
                            sf::FloatRect leader_Rect = Leader_title.getLocalBounds();
                            Leader_title.setOrigin(leader_Rect.left + leader_Rect.width / 2.0f,
                                                   leader_Rect.top + leader_Rect.height / 2.0f);
                            Leader_title.setPosition(16 * config_cols / 2.0f, 16 * config_rows / 2.0f - 120);


                            sf::Text Player_list;
                            Player_list.setFont(font);
                            Player_list.setCharacterSize(18);
                            Player_list.setStyle(sf::Text::Bold);
                            Player_list.setFillColor(sf::Color::White);
                            Player_list.setString(print_leaderboard(players));
                            sf::FloatRect list_Rect = Player_list.getLocalBounds();
                            Player_list.setOrigin(list_Rect.left + list_Rect.width / 2.0f,
                                                  list_Rect.top + list_Rect.height / 2.0f);
                            Player_list.setPosition(16 * config_cols / 2.0f, 16 * config_rows / 2.0f + 20);

                            // while the leaderboard is open
                            while (leaderboard.isOpen()) {

                                sf::Event event2;
                                while (leaderboard.pollEvent(event2)) {
                                    if (event2.type == sf::Event::Closed) {
                                        hasWon = false;
                                        leaderboard.close();
                                    }
                                }
                                //game.display();
                                leaderboard.clear(sf::Color::Blue);
                                leaderboard.draw(Leader_title);
                                leaderboard.draw(Player_list);
                                leaderboard.display();
                            }


                        }


                    } else if (lost) {
                        happy.Set_Texture(textures["sad"]);
                        if (currentTile.mine and !currentTile.flagged) {
                            currentTile.reveal_sprite.setTexture(textures["revealed"]);
                            currentTile.reveal_sprite.setPosition(32 * i, 32 * j);
                            currentTile.mine_sprite.setPosition(32 * i, 32 * j);
                            gameWindow.draw(currentTile.reveal_sprite);
                            gameWindow.draw(currentTile.mine_sprite);

                        }

                    }

                    if (reset) {
                        reset = false;
                        firsttimearound = true;

                        if (win) {
                            win = false;
                            happy.Set_Texture(textures["happy_face"]);
                        } else {
                            lost = false;
                            happy.Set_Texture(textures["happy_face"]);
                        }
                    }


                }
            }

            sf::Event event;

            while (gameWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    gameWindow.close();
                }

                if (event.type == sf::Event::MouseButtonPressed) {

                    sf::Mouse mouse;
                    auto coordinates = mouse.getPosition(gameWindow);     // coordinates of click

                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {   // if it is a left click
                        //If restart button is clicked
                        auto Happy_Rect = happy.open().getGlobalBounds();
                        if (Happy_Rect.contains(coordinates.x, coordinates.y)) {
                            paused = false;
                            reset = true;
                            happy.Set_Texture(textures["happy_face"]);
                            board.ResetBoard(textures["unrevealed"], config_cols, config_rows, config_mines);

                            flags_placed = 0;
                            flag_count = 0;
                            reveal_count = 0;

                            // Reset timer
                            start_time = chrono::high_resolution_clock::now();

                            // Reset counter
                            neg = false;
                            count0.Set_Texture(clock[count_val0]);
                            count1.Set_Texture(clock[count_val1]);
                            count2.Set_Texture(clock[count_val2]);
                        }
                        // If leaderboard button is clicked
                        auto Leaderboard_Rect = leader_button.open().getGlobalBounds();
                        if (Leaderboard_Rect.contains(coordinates.x, coordinates.y)) {
                            leader = !leader;
                            for (int i = 0; i < board.cols; i++) {
                                for (int j = 0; j < board.rows; j++) {
                                    sf::Sprite temp(textures["revealed"]);
                                    temp.setPosition(board.tiles[i][j].reveal_sprite.getPosition());
                                    gameWindow.draw(temp);
                                }
                            }
                            gameWindow.draw(sec_col2.open());
                            gameWindow.draw(sec_col1.open());
                            gameWindow.draw(minute_col2.open());
                            gameWindow.draw(minute_col1.open());

                            if (neg) {
                                gameWindow.draw(negative.open());
                            }
                            gameWindow.draw(happy.open());
                            gameWindow.draw(pause.open());
                            gameWindow.draw(leader_button.open());
                            gameWindow.draw(debug.open());
                            gameWindow.draw(count0.open());
                            gameWindow.draw(count1.open());
                            gameWindow.draw(count2.open());

                            gameWindow.display();
                            if (leader) {
                                pauseTime = chrono::high_resolution_clock::now();

                                // render the leaderboard window
                                sf::RenderWindow leaderboard(sf::VideoMode(16 * config_cols, (16 * config_rows) + 50),
                                                             "Leaderboard", sf::Style::Close | sf::Style::Titlebar);
                                sf::Text Leader_title;
                                Leader_title.setFont(font);
                                Leader_title.setCharacterSize(20);
                                Leader_title.setFillColor(sf::Color::White);
                                Leader_title.setString("LEADERBOARD");
                                Leader_title.setStyle(sf::Text::Bold | sf::Text::Underlined);
                                sf::FloatRect leader_Rect = Leader_title.getLocalBounds();
                                Leader_title.setOrigin(leader_Rect.left + leader_Rect.width / 2.0f,
                                                       leader_Rect.top + leader_Rect.height / 2.0f);
                                Leader_title.setPosition(16 * config_cols / 2.0f, 16 * config_rows / 2.0f - 120);


                                sf::Text Player_list;
                                Player_list.setFont(font);
                                Player_list.setCharacterSize(18);
                                Player_list.setStyle(sf::Text::Bold);
                                Player_list.setFillColor(sf::Color::White);
                                Player_list.setString(print_leaderboard(players));
                                sf::FloatRect list_Rect = Player_list.getLocalBounds();
                                Player_list.setOrigin(list_Rect.left + list_Rect.width / 2.0f,
                                                      list_Rect.top + list_Rect.height / 2.0f);
                                Player_list.setPosition(16 * config_cols / 2.0f, 16 * config_rows / 2.0f + 20);

                                // while the leaderboard is open
                                while (leaderboard.isOpen()) {
                                    sf::Event event2;
                                    while (leaderboard.pollEvent(event2)) {
                                        if (event2.type == sf::Event::Closed) {
                                            leader = false;
                                            leaderboard.close();
                                            auto resumeTime = chrono::high_resolution_clock::now();   // calculate amount of time that leaderboard was open
                                            elapsed_paused_time = elapsed_paused_time += chrono::duration_cast<chrono::seconds>(
                                                    resumeTime - pauseTime).count();

                                        }
                                    }
                                    //game.display();
                                    leaderboard.clear(sf::Color::Blue);
                                    leaderboard.draw(Leader_title);
                                    leaderboard.draw(Player_list);
                                    leaderboard.display();

                                }

                            }
                        }


                        if (!lost and !win) {
                            // if Pause button is clicked
                            auto Pause_Rect = pause.open().getGlobalBounds();
                            if (Pause_Rect.contains(coordinates.x, coordinates.y)) {
                                paused = !paused;
                                if (paused) {
                                    pause.Set_Texture(textures["play"]);
                                    // Pause Timer
                                    pauseTime = chrono::high_resolution_clock::now();

                                } else {
                                    pause.Set_Texture(textures["pause"]);
                                    auto resumeTime = chrono::high_resolution_clock::now();   // calculate amount of time that it was paused;
                                    elapsed_paused_time = elapsed_paused_time += chrono::duration_cast<chrono::seconds>(
                                            resumeTime - pauseTime).count();

                                }

                            }


                            if (!paused) {          // checks if game is paused
                                for (int i = 0; i < board.cols; i++) {
                                    for (int j = 0; j < board.rows; j++) {
                                        auto Tile_rect = board.tiles[i][j].reveal_sprite.getGlobalBounds();         // gets global bounds of an individual tile
                                        if (Tile_rect.contains(coordinates.x, coordinates.y)) {
                                            if (board.tiles[i][j].mine and
                                                !board.tiles[i][j].flagged) {            // checks if a tile is a mine and that if it is flagged
                                                board.tiles[i][j].SetReveal(
                                                        textures["revealed"]);              // reveals the tile
                                                lost = true;

                                            } else if (!board.tiles[i][j].flagged) {                     // checks if a tile is flagged
                                                if (board.tiles[i][j].neighbor_bombs != 0) {
                                                    board.tiles[i][j].SetReveal(textures["revealed"]);
                                                    board.tiles[i][j].SetNumber(
                                                            numbers[board.tiles[i][j].neighbor_bombs]);
                                                } else if (board.tiles[i][j].neighbor_bombs == 0) {
                                                    board.Reveal_around(textures["revealed"], numbers, i, j);
                                                }

                                            }
                                        }
                                    }
                                }

                                // if debug button is clicked
                                auto Debug_Rect = debug.open().getGlobalBounds();
                                if (Debug_Rect.contains(coordinates.x, coordinates.y)) {
                                    debug_pushed = !debug_pushed;           // flips the debug boolean value
                                }

                            }
                        }
                    } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                        if (!win and !lost) {
                            for (int i = 0; i < board.cols; i++) {
                                for (int j = 0; j < board.rows; j++) {
                                    auto Flag_rect = board.tiles[i][j].reveal_sprite.getGlobalBounds();
                                    if (Flag_rect.contains(coordinates.x, coordinates.y) and
                                        !board.tiles[i][j].revealed) {
                                        if (board.tiles[i][j].flagged) {
                                            flags_placed--;
                                        } else {
                                            flags_placed++;
                                        }
                                        board.tiles[i][j].SetFlag();

                                        counter_val = config_mines - flags_placed;
                                        if (counter_val < 0) {
                                            neg = true;
                                            new_val0 = -1 * (counter_val / 100);
                                            new_val1 = -1 * (counter_val / 10);
                                            new_val2 = -1 * (counter_val % 10);
                                        } else {
                                            neg = false;
                                            if (digi_num == 1) {
                                                new_val2 = config_mines - flags_placed;
                                            } else if (digi_num == 2) {
                                                new_val1 = counter_val / 10;
                                                new_val2 = counter_val % 10;

                                            } else if (digi_num == 3) {
                                                new_val0 = counter_val / 100;
                                                new_val1 = (counter_val % 100) / 10;
                                                new_val2 = counter_val % 10;
                                            }

                                        }
                                        // Change the counter texture
                                        count0.Set_Texture(clock[new_val0]);
                                        count1.Set_Texture(clock[new_val1]);
                                        count2.Set_Texture(clock[new_val2]);
                                    }
                                }
                            }
                        }
                    }
                }
            }


            if (!win and !lost) {
                minute_col1.Set_Texture(clock[minutes0]);
                minute_col2.Set_Texture(clock[minutes1]);
                sec_col1.Set_Texture(clock[seconds0]);
                sec_col2.Set_Texture(clock[seconds1]);
            }

            gameWindow.draw(sec_col2.open());
            gameWindow.draw(sec_col1.open());
            gameWindow.draw(minute_col2.open());
            gameWindow.draw(minute_col1.open());

            if (neg) {
                gameWindow.draw(negative.open());
            }
            gameWindow.draw(happy.open());
            gameWindow.draw(pause.open());
            gameWindow.draw(leader_button.open());
            gameWindow.draw(debug.open());
            gameWindow.draw(count0.open());
            gameWindow.draw(count1.open());
            gameWindow.draw(count2.open());

            game.draw(minute_col1.open());
            game.draw(minute_col2.open());
            game.draw(sec_col1.open());
            game.draw(sec_col2.open());


            gameWindow.display();

        }


    }
};

 if (gameWindowOpen) {
auto game_runtime = std::chrono::duration_cast<std::chrono::seconds>(chrono::high_resolution_clock::now() - start_time);
int total_time = game_runtime.count(); // necessary to subtract elapsed time later because "game_runtime.count()" is const



        while (gameWindow.isOpen()) {
            sf::Event event;
            gameWindow.clear(sf::Color::White);

            while (gameWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    gameWindow.close();
                }

            }


            gameWindow.display();

        }

        //gameWindowOpen = false;


    }
//return 0;

*/


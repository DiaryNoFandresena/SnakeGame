#include <iostream>
#include<cstdlib>																									//bibliothèque pour le temps/heure
 #include <SFML/Graphics.hpp>																						//bibliothèque pour l'interface
using namespace std;
using namespace sf;

int Size=20;																										//taille d'une carreau/case du background
int width = ((Size*Size)+200), height =(Size*Size) ;	
int num = 2, dir=1 ;
int number(0);		
enum { HOME, GAME, GAMEOVER };																						//largueur (x,sens vers la droite)= hauteur (y,sens vers le bas)= 400

struct snake{
	int x,y;
}s[100];

struct  fruit{
	int x,y;
} f;

void positionMiam(fruit& f){
	srand(time(0));																									//position nourriture  al�atoirement
	f.x = rand() % Size;
    f.y = rand() % Size;
	cout << " positionMiam function is ready !! " << endl;
}

void eating(fruit& f, snake s[100]){
	if (  s[0].x == f.x    &&     s[0].y == f.y  ){																	// si le position du serpent = position nourriture --> serpent s'allonge et nourriture change de place
		num++;
		number+=5;
		positionMiam(f);
		cout << " eating function is ready !! " << endl;
	}
}

int getStatus(snake s[100] ){
	for ( int i=2; i<100 ; i++){
		if (  s[0].x == s[i].x    &&     s[0].y == s[i].y  ){
			cout<<"getStatus is ready !!!!! " << endl;
			num=2;	
			return GAMEOVER;																						// si le position du serpent = position nourriture --> serpent s'allonge et nourriture change de place
		}
	}
	return GAME;
}

void overBox(snake s[100]){																							//en cas de sortie  du zone, le serpent se teleporte vers l'oppos� du zone
	if (   s[0].x<0   ) s[0].x=Size  ;
	if (   s[0].x>Size) s[0].x=0  ;
	if (   s[0].y<0   ) s[0].y=Size  ;
	if (   s[0].y>Size) s[0].y=0  ;
	cout << " overBox function is ready !! " << endl;
}

void deplacement(fruit& f, snake s[100], int& gameStatus){															//tous les actions/deplacement que le serpent peut faire et ses effets
	for (int i=num ; i>0 ; i--){
		s[i].x = s[i-1].x ;
		s[i].y = s[i-1].y ;
	}
	if (dir==0) s[0].y-- ;
	if (dir==1) s[0].x++ ;
	if (dir==2) s[0].y++ ;
	if (dir==3) s[0].x-- ;

	gameStatus = getStatus(s);

	eating(f,s);
	overBox(s);
	cout << " deplacement function is ready !! " << endl;
}

void InitText(Text& texte, Font& police, string word, int size, int x , int y){
	texte.setFont(police);
	texte.setString(word);
	texte.setCharacterSize(size);
	texte.Transformable::setPosition(x,y);
	texte.setFillColor(Color::Red);
	cout << " InitText function is ready !! " << endl;	
}


int main()
{
	RenderWindow  screen(VideoMode (width,height)," Anaconda" , Style::Titlebar | Style::Resize | sf::Style::Close );	//creer un fenetre de 400x400, titre"Anaconda", avec barre de menu + bouton fermer et agrandir;
	screen.setFramerateLimit(60);																						//vitesse du trame : 60img/s;

		sf::Texture textureHome;
		textureHome.loadFromFile("images/home.png");
		Sprite spriteHome(textureHome);	

		sf::Texture textureGame;
		textureGame.loadFromFile("images/game.png");
		Sprite spriteGame(textureGame);

		sf::Texture textureGameOver;
		textureGameOver.loadFromFile("images/gameover.png");
		Sprite spriteGameOver(textureGameOver);

				

        // RectangleShape box( Vector2f(Size,Size));
		CircleShape box( 10);
        box.setFillColor(Color(51, 51, 51));
		box.setRadius(10);
		CircleShape  miam(10);
		miam.setRadius(10);
        miam.setFillColor(Color(193, 39, 45));
		Font font;
		Text score;

		int gameStatus(HOME);

		snake s[100];
			s[0].x=4;
			s[0].y=0;
			s[1].x=3;
			s[1].y=0;
		fruit f;
		int vitesse=100;
		Clock timer;	
		string mot;																							//creer variable de type clock

        positionMiam(f);
																																
	while ( screen.isOpen()  ){																				//tant que fenetre ouvert;
		Event action;																						//classe de type Event pour stocker l'evenement en cours
		while (  screen.pollEvent(action)  ){		
			if (action.type == Event::KeyPressed) {
				if (gameStatus != GAME) 
					gameStatus=GAME;
				else{
					switch (action.key.code)
					{
					case Keyboard::Key::Up:
						dir=0;
						break;
					case Keyboard::Key::Right:
						dir=1;
						break;
					case Keyboard::Key::Down:
						dir=2;
						break;
					case Keyboard::Key::Left:
						dir=3;
						break;
					
					default:
						break;
					}
				}	

				if ((action.key.code == Keyboard::Key::Escape) || (action.type == Event::Closed) ) 
					screen.close();																			//touche escape --> quitter															//
			}
			if (action.type == Event::Closed) 
				screen.close();
		}

		screen.clear();				
		switch (gameStatus)
		{
		case GAME:
			screen.clear();	
			screen.draw(spriteGame);
			if(timer.getElapsedTime().asMilliseconds() > vitesse){
				deplacement(f,s, gameStatus);
				timer.restart();
			}
			if( num%4==0) {
				vitesse= 100-((num/10)*40);	
			}

			for ( int i=0 ; i<num ; i++){
				box.setPosition(   s[i].x * Size  ,   s[i].y * Size   );
				screen.draw(box);
			}
			miam.setPosition(   f.x * Size  ,   f.y * Size   );
			screen.draw(miam);
			screen.draw(score);																										//
			screen.display();	
		
			if (!font.loadFromFile("font/Mega Brush.otf")){
					cout<<"erreur";
			}	
			else {
				mot=to_string(number);
				InitText(score, font, mot, 40, 479, 134);
				score.setFillColor(Color(193, 39, 45));
				
			}
			break;

		case HOME:
			screen.draw(spriteHome);
			break;

		case GAMEOVER:
			screen.draw(spriteGameOver);
			number=0;
			break;

		default:
			break;
		}
		screen.display();																					//affiche fenetre
	}
	return 0;
}

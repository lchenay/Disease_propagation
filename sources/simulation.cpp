#include "simulation.h"



// Cr�e une simulation

Simulation::Simulation()
{
	pixels_size = 0;
	vaccinated_percentage = 0.;
	nb_sick = 0;
	death_rate = 0.;
	contagious_level = 0.;
	time_before_death = 0;
	time_before_cure = 0;
	population.clear();
}



// Cr�e une simulation � partir d'une autre

Simulation::Simulation(const Simulation& simulation)
{
	pixels_size = simulation.pixels_size;
	vaccinated_percentage = simulation.vaccinated_percentage;
	nb_sick = simulation.nb_sick;
	death_rate = simulation.death_rate;
	contagious_level = simulation.contagious_level;
	time_before_death = simulation.time_before_death;
	time_before_cure = simulation.time_before_death;
	person = simulation.person;
	population = simulation.population;
	sprite = simulation.sprite;
}



// Cr�e une simulation � partir des donn�es du menu

Simulation::Simulation(const Menu& menu)
{
	pixels_size = menu.variables[0].int_value;
	vaccinated_percentage = menu.variables[1].double_value / 100.;
	nb_sick = menu.variables[2].int_value;
	death_rate = menu.variables[3].double_value / 100.;
	contagious_level = menu.variables[4].double_value / 100.;
	time_before_death = menu.variables[5].int_value;
	time_before_cure = menu.variables[6].int_value;

	switch (pixels_size)
	{
	case 7:  pixels_size = 6;  break;
	case 9:  pixels_size = 8;  break;
	case 11: pixels_size = 10; break;
	case 13: pixels_size = 12; break;
	case 14: pixels_size = 15; break;
	case 16: pixels_size = 15; break;
	case 17: pixels_size = 15; break;
	case 18: pixels_size = 20; break;
	case 19: pixels_size = 20; break;
	}

	texture.create(WIDTH, HEIGHT);
	texture.clear(sf::Color::Black);
	texture.display();
	sprite = sf::Sprite(texture.getTexture());

	population.clear();
	std::vector<Person> temp;
	temp.clear();

	for (int i = 0; i < WIDTH / pixels_size; i++)
	{
		for (int j = 0; j < HEIGHT / pixels_size; j++)
			temp.push_back(Person(vaccinated_percentage, death_rate));

		population.push_back(temp);
		temp.clear();
	}

	int x;
	int y;

	for (int i = 0; i < nb_sick; i++)
	{
		x = rand() % population.size();
		y = rand() % population.front().size();

		while (population[x][y].health == sick)
		{
			x = rand() % population.size();
			y = rand() % population.front().size();
		}

		population[x][y].health = sick;
	}

	person.setSize(sf::Vector2f(pixels_size, pixels_size));

	draw_population();
}



// Assignation

void Simulation::operator=(const Simulation& simulation)
{
	pixels_size = simulation.pixels_size;
	vaccinated_percentage = simulation.vaccinated_percentage;
	nb_sick = simulation.nb_sick;
	death_rate = simulation.death_rate;
	contagious_level = simulation.contagious_level;
	time_before_death = simulation.time_before_death;
	time_before_cure = simulation.time_before_death;
	person = simulation.person;
	population = simulation.population;
	sprite = simulation.sprite;
}



// Recrr�e une simulation � partir des donn�es du menu

void Simulation::restart(const Menu& menu)
{
	pixels_size = menu.variables[0].int_value;
	vaccinated_percentage = menu.variables[1].double_value / 100.;
	nb_sick = menu.variables[2].int_value;
	death_rate = menu.variables[3].double_value / 100.;
	contagious_level = menu.variables[4].double_value / 100.;
	time_before_death = menu.variables[5].int_value;
	time_before_cure = menu.variables[6].int_value;

	switch (pixels_size)
	{
	case 7:  pixels_size = 6;  break;
	case 9:  pixels_size = 8;  break;
	case 11: pixels_size = 10; break;
	case 13: pixels_size = 12; break;
	case 14: pixels_size = 15; break;
	case 16: pixels_size = 15; break;
	case 17: pixels_size = 15; break;
	case 18: pixels_size = 20; break;
	case 19: pixels_size = 20; break;
	}

	texture.clear(sf::Color::Black);
	texture.display();
	sprite = sf::Sprite(texture.getTexture());

	population.clear();
	std::vector<Person> temp;
	temp.clear();

	for (int i = 0; i < WIDTH / pixels_size; i++)
	{
		for (int j = 0; j < HEIGHT / pixels_size; j++)
			temp.push_back(Person(vaccinated_percentage, death_rate));

		population.push_back(temp);
		temp.clear();
	}

	int x;
	int y;

	for (int i = 0; i < nb_sick; i++)
	{
		x = rand() % population.size();
		y = rand() % population.front().size();

		while (population[x][y].health == sick)
		{
			x = rand() % population.size();
			y = rand() % population.front().size();
		}

		population[x][y].health = sick;
	}

	person.setSize(sf::Vector2f(pixels_size, pixels_size));

	draw_population();
}



// Affiche une personne

void Simulation::draw_person(int x, int y)
{
	person.setPosition(x * pixels_size, y * pixels_size);

	switch (population[x][y].health)
	{
	case vaccinated:
		person.setFillColor(VACCINATED_COLOR);
		break;

	case healthy:
		person.setFillColor(HEALTHY_COLOR);
		break;

	case sick:
		person.setFillColor(SICK_COLOR);
		break;

	case dead:
		person.setFillColor(DEAD_COLOR);
		break;
	}

	texture.draw(person);
}



// Affiche la population

void Simulation::draw_population()
{
	for (int i = 0; i < population.size(); i++)
	{
		for (int j = 0; j < population.front().size(); j++)
			draw_person(i, j);
	}
}



// Enl�ve la s�curit� qui emp�che de prendre en compte plusieurs fois une personne

void Simulation::refresh_population()
{
	for (int i = 0; i < population.size(); i++)
	{
		for (int j = 0; j < population.front().size(); j++)
			population[i][j].new_sick = false;
	}
}



// Emp�che de choisir une case en dehors du tableau

bool Simulation::anti_segfault(int x, int y)
{
	return (x >= 0 and y >= 0 and x < population.size() and y < population.front().size());
}



// Met � jour les personnes non vaccin�es

void Simulation::update_healthy(int x, int y)
{
	bool is_sick = false;

	if (anti_segfault(x - 1, y - 1) and population[x - 1][y - 1].health == sick and !population[x - 1][y - 1].new_sick) is_sick = rand_probability(contagious_level);
	if (anti_segfault(x - 1, y) and population[x - 1][y].health == sick and !population[x - 1][y].new_sick) is_sick = rand_probability(contagious_level);
	if (anti_segfault(x - 1, y + 1) and population[x - 1][y + 1].health == sick and !population[x - 1][y + 1].new_sick) is_sick = rand_probability(contagious_level);
	if (anti_segfault(x, y - 1) and population[x][y - 1].health == sick and !population[x][y - 1].new_sick) is_sick = rand_probability(contagious_level);
	if (anti_segfault(x, y + 1) and population[x][y + 1].health == sick and !population[x][y + 1].new_sick) is_sick = rand_probability(contagious_level);
	if (anti_segfault(x + 1, y - 1) and population[x + 1][y - 1].health == sick and !population[x + 1][y - 1].new_sick) is_sick = rand_probability(contagious_level);
	if (anti_segfault(x + 1, y) and population[x + 1][y].health == sick and !population[x + 1][y].new_sick) is_sick = rand_probability(contagious_level);
	if (anti_segfault(x + 1, y + 1) and population[x + 1][y + 1].health == sick and !population[x + 1][y + 1].new_sick) is_sick = rand_probability(contagious_level);

	if (is_sick)
	{
		population[x][y].health = sick;
		population[x][y].new_sick = true;

		draw_person(x, y);
	}
}



// Met � jour les personnes malades

void Simulation::update_sick(int x, int y)
{
	population[x][y].nb_sick_days++;

	if (population[x][y].resistant)
	{
		if (population[x][y].nb_sick_days == time_before_cure)
		{
			population[x][y].health = vaccinated;
			draw_person(x, y);
		}
	}

	else
	{
		if (population[x][y].nb_sick_days == time_before_death)
		{
			population[x][y].health = dead;
			draw_person(x, y);
		}
	}
}



// Met � jour la population

void Simulation::update()
{
	for (int i = 0; i < population.size(); i++)
	{
		for (int j = 0; j < population.front().size(); j++)
		{
			if (population[i][j].health == healthy)
				update_healthy(i, j);

			if (population[i][j].health == sick)
				update_sick(i, j);
		}
	}

	refresh_population();
}



// Affiche la simulation

void Simulation::draw(sf::RenderWindow& window)
{
	texture.display();
	sprite = sf::Sprite(texture.getTexture());
	window.draw(sprite);
}

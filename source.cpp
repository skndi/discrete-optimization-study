#include <string>
#include <fstream>
#include <vector>	
#include <queue>
#include <iostream>
#include <map>
#include <stack>
#include <set>

struct Item {
	int id;
	int value;
	int weight;
	Item() {
		id = 0;
		value = 0;
		weight = 0;
	}
	Item(int i, int v, int w) {
		id = i;
		value = v;
		weight = w;
	}
};

void greedy_best_first(int item_count, int knapsack_capacity, std::vector<Item> items, std::vector<bool> taken, bool (*f)(Item, Item)) {
	std::sort(items.begin(), items.end(), f);
	int curr_capacity = 0;
	int value = 0;
	int counter = 0;

	while (curr_capacity + items[counter].weight <= knapsack_capacity) {
		taken[items[counter].id] = 1;
		value += items[counter].value;
		curr_capacity += items[counter].weight;
		counter++;
	}
	std::cout << value << " " << 0 << '\n';
	for (int i = 0; i < item_count; i++) {
		std::cout << taken[i] << ' ';
	}
}

void highest_value(int item_count, int knapsack_capacity, std::vector<Item> items, std::vector<bool>& taken) {
	greedy_best_first(item_count, knapsack_capacity, items, taken, [](Item a, Item b) {return a.value > b.value;});
}

void lowest_weight(int item_count, int knapsack_capacity, std::vector<Item> items, std::vector<bool>& taken) {
	greedy_best_first(item_count, knapsack_capacity, items, taken, [](Item a, Item b) {return a.weight < b.weight;});
}

void best_wv_ratio(int item_count, int knapsack_capacity, std::vector<Item> items, std::vector<bool> taken) {
	greedy_best_first(item_count, knapsack_capacity, items, taken, [](Item a, Item b) {return ((double)a.value / a.weight) > ((double)b.value / b.weight);});
}

void dynamic_knapsack(int item_count, int knapsack_capacity, std::vector<Item> items, std::vector<bool> taken) {
	std::vector<int> curr_values(knapsack_capacity + 1);
	std::vector<std::vector<bool>> curr_taken;
	for (int i = 0; i <= knapsack_capacity; i++) {
		curr_taken.push_back(std::vector<bool>(taken.size(), false));
	}

	for (int j = 1; j <= item_count; j++){
		for(int i = knapsack_capacity; i >= 1; i--){
			if (items[j - 1].weight <= i) {
				if(curr_values[i] < curr_values[i - items[j - 1].weight] + items[j - 1].value){
					curr_values[i] = curr_values[i - items[j - 1].weight] + items[j - 1].value;
					curr_taken[i] = curr_taken[i - items[j - 1].weight];
					curr_taken[i][j - 1] = true;
				}
			}
		}
	}
	int final_value = curr_values[knapsack_capacity];
	std::cout << final_value << " " << 1 << '\n';
	for (int i = 0; i < item_count; i++) {
		std::cout << curr_taken[knapsack_capacity][i] << ' ';
	}
}

struct Instance {
	std::vector<bool> taken;
	int value;
	int room_left;
	int curr_item;
	Instance(int capacity) {
		value = 0;
		room_left = capacity;
		curr_item = 0;
	}
	void take_item(std::vector<Item> items, bool take) {
		if (take) {
			taken[items[curr_item].id] = true;
			room_left -= items[curr_item].weight;
			value += items[curr_item].value;
			curr_item++;
		}
		else curr_item++;
	}
};

//NAPRAVI SET SUS RODITELQ NA VSEKI INSTANCE
void branch_and_bound(int item_count, int knapsack_capacity, std::vector<Item> items, std::vector<bool> taken) {
	int max_estimate = 0;
	Instance best_solution(knapsack_capacity);
	std::stack<Instance> instances;
	instances.push(Instance(knapsack_capacity));
	std::vector<Item>
	while (!instances.empty()) {
		Instance current_instance = instances.top();
		instances.pop();
		int estimate = bound(current_instance);
		if (estimate >= max_estimate) {
			best_solution = current_instance;
			max_estimate = estimate;
			if (current_instance.curr_item < items.size()) {
				std::pair<Instance, Instance> children = branch(current_instance, items);
				instances.push(children.first);
				instances.push(children.second);
			}
		}
	}
	int final_value = best_solution.value;
	std::cout << final_value << " " << 1 << '\n';
	for (int i = 0; i < item_count; i++) {
		std::cout << best_solution.taken[i] << ' ';
	}
}

std::pair<Instance, Instance> branch(const Instance& instance, const std::vector<Item>& item) {
	Instance inst_with = instance;
	inst_with.take_item(item, true);
	Instance inst_without = instance;
	inst_without.take_item(item, false);
}

int bound(const Instance& instance) {

}

int main(int argc, char* argv[]) {
	int item_count, knapsack_capacity;
	std::vector<Item> items;
	std::vector<bool> taken;
	std::string path = argv[1];
	std::ifstream file(path);
	file >> item_count >> knapsack_capacity;
	int v, w;

	for (int i = 0; i < item_count; i++) {
		file >> v >> w;
		items.push_back(Item(i, v, w));
		taken.push_back(false);
	}
	dynamic_knapsack(item_count, knapsack_capacity, items, taken);
	file.close();
	return 0;
}
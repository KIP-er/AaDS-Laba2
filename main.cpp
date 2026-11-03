import std;

template<typename T>
struct Node {
	T value;
	Node<T>* prev;
	Node<T>* next;
	Node(T value_) : value(value_), prev(nullptr), next(nullptr) {}
};
template<typename T>
class LinkedList {
	std::size_t count = 0;
	Node<T>* head = nullptr;
public:
	LinkedList() = default;
	LinkedList(const LinkedList<T>& other) {
		if (other.head) {
			Node<T>* cur = other.head;
			do {
				push_tail(cur->value);
				cur = cur->next;
			} while (cur != other.head);
		}
	}
	LinkedList(std::size_t count, unsigned int seed, T lower, T upper) {
		std::mt19937 engine(seed);
		for (std::size_t i = 0; i < count; ++i) {
			if constexpr (std::is_integral_v<T>) {
				std::uniform_int_distribution<T> dist(lower, upper);
				push_tail(dist(engine));
			}
			else {
				std::uniform_real_distribution<T> dist(lower, upper);
				push_tail(dist(engine));
			}
		}
	}
	~LinkedList() {
		while (head != nullptr) {
			pop_tail();
		}
	}
	LinkedList<T>& operator=(const LinkedList<T>& other) {
		if (this == &other) {
			return *this;
		}
		while (head != nullptr) {
			pop_tail();
		}
		if (other.head == nullptr) {
			return *this;
		}
		Node<T>* cur = other.head;
		do {
			push_tail(cur->value);
			cur = cur->next;
		} while (cur != other.head);
		return *this;
	}
	void push_tail(const T& val){
		Node<T>* new_node = new Node<T>(val);
		if (head != nullptr) {
			Node<T>*last = head->prev;
			new_node->next = head;
			new_node->prev = last;
			last->next = new_node;
			head->prev = new_node;
		}
		else {
			new_node->next = new_node;
			new_node->prev = new_node;
			head = new_node;
		}
		++count;
	}
	void push_tail(const LinkedList<T>& other) {
		if (other.head != nullptr) {
			Node<T>*cur = other.head;
			std::size_t total = other.count;
			for (std::size_t i = 0; i < total; ++i) {
				push_tail(cur->value);
				cur = cur->next;
			}
		}
	}
	void push_head(const T& val) {
		Node<T>* new_node = new Node<T>(val);
		if (head != nullptr) {
			Node<T>* last = head->prev;
			new_node->next = head;
			new_node->prev = last;
			last->next = new_node;
			head->prev = new_node;
			head = new_node;
		}
		else {
			new_node->next = new_node;
			new_node->prev = new_node;
			head = new_node;
		}
		++count;
	}
	void push_head(const LinkedList<T>& other) {
		if (other.head!=nullptr){
			Node<T>* cur = other.head->prev;
			std::size_t total = other.count;
			for (std::size_t i = 0; i < total; ++i) {
				push_head(cur->value);
				cur = cur->prev;
			}
		}
	}
	void pop_head() {
		if (head == nullptr) {
			throw std::logic_error("List is empty");
		}
		else if (head->next == head) {
			delete head;
			head = nullptr;
		}
		else {
			Node<T>* last = head->prev;
			Node<T>* new_head = head->next;
			last->next = new_head;
			new_head->prev = last;
			delete head;
			head = new_head;
		}
		--count;
	}
	void pop_tail() {
		if (head == nullptr) {
			throw std::logic_error("List is empty");
		}
		else if (head->next == head) {
			delete head;
			head = nullptr;
		}
		else {
			Node<T>* tail = head->prev;
			Node<T>* new_tail = tail->prev;
			new_tail->next = head;
			head->prev = new_tail;
			delete tail;
			tail = nullptr;
		}
		--count;
	}
	void delete_node(const T& val) {
		if (head == nullptr) {
			return;
		}
		else {
			const std::size_t total = count;
			Node<T>* cur = head;
			for (std::size_t i = 0; i < total; ++i) {
				Node<T>* next_cur = cur->next;
				Node<T>* prev_cur = cur->prev;
				if (cur->value == val) {
					if (cur == next_cur) {
						delete cur;
						head = nullptr;
						--count;
						return;
					}
					Node<T>* del_cur = cur;
					prev_cur->next = next_cur;
					next_cur->prev = prev_cur;
					if (cur == head) {
						head = next_cur;
					}
					cur = next_cur;
					delete del_cur;
					del_cur = nullptr;
					--count;
					continue;
				}
				cur = cur->next;
			}
		}
	}
	T& operator[](std::size_t index) {
		if (count == 0) {
			throw std::logic_error("List is empty");
		}
		Node<T>* cur = head;
		for (std::size_t i = 0; i < index % count; ++i) {
			cur = cur->next;
		}
		return cur->value;
	}
	const T& operator[](std::size_t index) const {
		if (count == 0) {
			throw std::logic_error("List is empty");
		}
		const Node<T>* cur = head;
		for (std::size_t i = 0; i < index % count; ++i) {
			cur = cur->next;
		}
		return cur->value;
	}
	std::size_t get_count() const {
		return count;
	}
	Node<T>* get_head() {
		return head;
	}
	const Node<T>* get_head() const {
		return head;
	}
	void set_head(Node<T>*h) {
		head = h;
	}
};
template<typename T>
std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list) {
	std::size_t size = list.get_count();
	for (std::size_t i = 0; i < size; ++i) {
		os << list[i] << " ";
	}
	return os;
}
template<typename T>
void reverse_list(LinkedList<T>& list) {
	std::size_t size = list.get_count();
	if (size < 2) {
		return;
	}
	Node<T>* cur = list.get_head();
	Node<T>* new_head = cur->prev;
	Node<T>* next_node = nullptr;
	for (std::size_t i = 0; i < size; ++i) {
		next_node = cur->next;
		std::swap(cur->next, cur->prev);
		cur = next_node;
	}
	list.set_head(new_head);
}
void print_menu() {
	std::cout << "\n========== LinkedList Menu ==========\n";
	std::cout << "[1]  Default constructor (empty list)\n";
	std::cout << "[2]  Copy constructor (copy of current list or the list we are copying)\n";
	std::cout << "[3]  Random-fill constructor (dimension, lower/upper bounds)\n";
	std::cout << "[4]  Assignment operator= (copy another list or the list into which we are copying)\n";
	std::cout << "[5]  push_tail: add element to the end\n";
	std::cout << "[6]  push_tail: add another list to the end\n";
	std::cout << "[7]  push_head: add element to the beginning\n";
	std::cout << "[8]  push_head: add another list to the beginning\n";
	std::cout << "[9]  pop_head: remove element from the beginning\n";
	std::cout << "[10] pop_tail: remove element from the end\n";
	std::cout << "[11] delete_node: delete all elements equal to value\n";
	std::cout << "[12] Read element by index (operator[] const)\n";
	std::cout << "[13] Write element by index (operator[] non-const)\n";
	std::cout << "[14] Print the list (operator<<)\n";
	std::cout << "[15] Reverse list (reverse_list)\n";
	std::cout << "[16] Exit\n";
	std::cout << "=====================================\n";
	std::cout << "Your choice: ";
}
int read_slot() {
	int slot = 0;
	while (true) {
		std::cout << "Choose list (1 or 2): ";
		std::cin >> slot;
		if (slot == 1 || slot == 2) {
			return slot - 1;
		}
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Please enter 1 or 2.\n";
	}
}
template<typename T>
T read_value(const char* prompt) {
	T value{};
	while (true) {
		std::cout << prompt << ": ";
		if (std::cin >> value) {
			return value;
		}
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid input, please try again.\n";
	}
}
std::size_t read_size(const char* prompt) {
	int size = 0;
	while (true) {
		std::cout << prompt;
		if (std::cin>> size && size >= 0) {
			return static_cast<std::size_t>(size);
		}
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "The value must be a positive integer.\n";
	}
}
unsigned int read_seed(const char* prompt) {
	int seed = 0;
	while (true) {
		std::cout << prompt;
		std::cin >> seed;
		if (std::cin >> seed && seed >= 0) {
			return static_cast<unsigned int>(seed);
		}
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "The value must be a positive integer.\n";
	}
}
template<typename T>
void run_menu() {
	LinkedList<T>* lists[2] = { nullptr, nullptr };
	while (true) {
		print_menu();
		int choice = 0;
		if (!(std::cin >> choice)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input, please try again.\n";
			continue;
		}
		try {
			switch (choice) {
			case 1: {
				int slot = read_slot();
				delete lists[slot];
				lists[slot] = new LinkedList<T>();
				std::cout << "List " << slot + 1 << " = " << *lists[slot] << "\n";
				break;
			}
			case 2: {
				int slot = read_slot();
				if (!(lists[slot])) {
					std::cout << "List " << slot + 1 << " has not been created yet.\n";
					break;
				}
				delete lists[1 - slot];
				lists[1 - slot] = new LinkedList<T>(*lists[slot]);
				std::cout << "List " << 2 - slot << " = " << *lists[1 - slot] << "\n";
				break;
			}
			case 3: {
				int slot = read_slot();
				std::size_t size = read_size("List dimension: ");
				T lower = read_value<T>("Lower bound");
				T upper = read_value<T>("Upper bound");
				if (lower > upper) {
					std::cout << "The lower bound bigger the upper bound.\n";
					break;
				}
				unsigned int seed = static_cast<unsigned int>(read_seed("Enter the seed: "));
				delete lists[slot];
				lists[slot] = new LinkedList<T>(size, seed, lower, upper);
				std::cout << "List " << slot + 1 << " = " << *lists[slot] << '\n';
				break;
			}
			case 4: {
				int slot = read_slot();
				if (!lists[slot] || !lists[1 - slot]) {
					std::cout << "Both lists must be created first.\n";
					break;
				}
				*lists[slot] = *lists[1 - slot];
				std::cout << "List " << slot + 1 << " = " << *lists[slot] << '\n';
				break;
			}
			case 5: {
				int slot = read_slot();
				if (!(lists[slot])) {
					std::cout << "List " << slot + 1 << " has not been created yet.\n";
					break;
				}
				T value = read_value<T>("Value");
				lists[slot]->push_tail(value);
				std::cout << "List " << slot + 1 << " = " << *lists[slot] << '\n';
				break;
			}
			case 6: {
				int slot = read_slot();
				if (!lists[slot] || !lists[1 - slot]) {
					std::cout << "Both lists must be created first";
					break;
				}
				lists[slot]->push_tail(*lists[1 - slot]);
				std::cout << "List " << slot + 1 << " = " << *lists[slot] << '\n';
				break;
			}
			case 7: {
				int slot = read_slot();
				if (!(lists[slot])) {
					std::cout << "List " << slot + 1 << " has not been created yet.\n";
					break;
				}
				T value = read_value<T>("Value");
				lists[slot]->push_head(value);
				std::cout << "List " << slot + 1 << " = " << *lists[slot] << '\n';
				break;
			}
			case 8: {
				int slot = read_slot();
				if (!lists[slot] || !lists[1 - slot]) {
					std::cout << "Both lists must be created first";
					break;
				}
				lists[slot]->push_head(*lists[1 - slot]);
				std::cout << "List " << slot + 1 << " = " << *lists[slot] << '\n';
				break;
			}
			case 9: {
				int slot = read_slot();
				if (!lists[slot]) {
					std::cout << "List " << slot + 1 << " has not been created yet.\n";
					break;
				}
				lists[slot]->pop_head();
				std::cout << "List " << slot + 1 << " = " << *lists[slot] << "\n";
				break;
			}
			case 10: {
				int slot = read_slot();
				if (!lists[slot]) {
					std::cout << "List " << slot + 1 << " has not been created yet.\n";
					break;
				}
				lists[slot]->pop_tail();
				std::cout << "List " << slot + 1 << " = " << *lists[slot] << "\n";
				break;
			}
			case 11: {
				int slot = read_slot();
				if (!lists[slot]) {
					std::cout << "List " << slot + 1 << " has not been created yet.\n";
					break;
				}
				T value = read_value<T>("Value to delete");
				lists[slot]->delete_node(value);
				std::cout << "List " << slot + 1 << " = " << *lists[slot] << "\n";
				break;
			}
			case 12: {
				int slot = read_slot();
				if (!lists[slot]) {
					std::cout << "List " << slot + 1 << " has not been created yet.\n";
					break;
				}
				std::size_t index = read_size("Index: ");
				const LinkedList<T>& const_list = *lists[slot];
				std::cout << "Element = " << const_list[index] << '\n';
				break;
			}
			case 13: {
				int slot = read_slot();
				if (!lists[slot]) {
					std::cout << "List " << slot + 1 << " has not been created yet.\n";
					break;
				}
				std::size_t index = read_size("Index: ");
				T value = read_value<T>("New value");
				(*lists[slot])[index] = value;
				std::cout << "List " << slot + 1 << " = " << *lists[slot] << '\n';
				break;
			}
			case 14: {
				int slot = read_slot();
				if (!lists[slot]) {
					std::cout << "List " << slot + 1 << " has not been created yet.\n";
					break;
				}
				std::cout << "List " << slot + 1 << " = " << *lists[slot] << '\n';
				break;
			}
			case 15: {
				int slot = read_slot();
				if (!lists[slot]) {
					std::cout << "List " << slot + 1 << " has not been created yet.\n";
					break;
				}
				reverse_list(*lists[slot]);
				std::cout << "List " << slot + 1 << " = " << *lists[slot] << "\n";
				break;
			}
			case 16: {
				delete lists[0];
				delete lists[1];
				std::cout << "Goodbye\n";
				return;
			}
			default:
				std::cout << "Unknown menu item, please try again.\n";
				break;
			}
		}
		catch (const std::exception& e) {
			std::cout << "Error: " << e.what() << '\n';
		}
	}
}
int main(){
	std::cout << "Select the vector element type:\n";
	std::cout << "  [1] int\n";
	std::cout << "  [2] float\n";
	std::cout << "  [3] double\n";
	int type = 0;
	while (true) {
		std::cout << "Choice: ";
		std::cin >> type;
		if ((type > 0) and (type < 4)) {
			break;
		}
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid choice, please enter a number from 1 to 3.\n";
	}
	switch (type) {
	case 1:
		run_menu<int>();
		break;
	case 2:
		run_menu<float>();
		break;
	case 3:
		run_menu<double>();
		break;
	}
}
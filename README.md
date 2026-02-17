# 📚 Enhanced Library Tracker (C++ | DSA Project)

This is a command-line Library Management System built in C++ where I focused on applying real Data Structures and Algorithms instead of just writing basic CRUD logic.

The goal of this project was to simulate how a real library system works while optimizing performance using efficient data structures like hash maps, sets, heaps, and vectors.

---

## 💡 What This Project Does

This system allows you to:

- Add and remove books
- Add and remove members
- Issue and return books
- Search books by title, author, or availability
- Track member borrowing history
- View system-wide statistics
- Automatically recycle deleted IDs using a min-heap

Everything runs in a CLI-based interactive menu.

---

## 🧠 DSA Concepts I Used

Instead of using simple arrays everywhere, I used:

- `unordered_map` → For fast book & member lookup (O(1) average time)
- `unordered_set` → To track available books efficiently
- `vector` → To maintain borrowing records
- `priority_queue (min-heap)` → To recycle and reuse deleted IDs
- `sort()` → For ordered search results
-  `max_element()` → For optimized searching and statistics

This helped me understand how different data structures impact performance and scalability.

---

## 🚀 Why I Built This

I wanted to move beyond theoretical DSA problems and apply concepts in a real-world style system.

This project helped me:

- Think about state management
- Handle edge cases (e.g., removing issued books)
- Optimize lookups and searches
- Design structured logic for tracking records
- Understand how backend systems manage resources

---

## 📊 Features That Stand Out

- 🔁 Automatic ID reuse using a min-heap
- ⚡ O(1) average lookup using hash maps
- 📚 Sorted search results
- 🧾 Issue tracking with simulated timestamps
- 📈 Library statistics (total books, issued books, active members)
- 🔍 Flexible search functionality

---

## 🛠 Technologies Used

- C++
- STL (Standard Template Library)
- Hash Maps
- Sets
- Heaps
- Vectors
- Algorithms Library

---

## 👨‍💻 Author

Keshava Reddy

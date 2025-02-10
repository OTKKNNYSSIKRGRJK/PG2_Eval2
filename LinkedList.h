#pragma once

#include<memory>
#include<cassert>

template<typename T>
class LinkedList {
protected:
	T* Table_Element{ nullptr };
	bool* Table_IsActive{ nullptr };
	int* Table_Prev{ nullptr };
	int* Table_Next{ nullptr };

	std::uint32_t Capacity{ 32U };

	int Active_First{};
	int Active_Last{};
	int Inactive_First{};
	int Inactive_Last{};

	void Initialize(const std::uint32_t _capacity) {
		if (_capacity > 0) { Capacity = _capacity; }

		assert(Table_Element == nullptr);
		Table_Element = new T[Capacity];

		assert(Table_IsActive == nullptr);
		Table_IsActive = new bool[Capacity];

		assert(Table_Prev == nullptr);
		Table_Prev = new int[Capacity];

		assert(Table_Next == nullptr);
		Table_Next = new int[Capacity];

		std::memset(Table_IsActive, 0, sizeof(bool) * Capacity);
		for (std::uint32_t i{ 0U }; i < Capacity; ++i) {
			Table_Prev[i] = i - 1;
			Table_Next[i] = i + 1;
		}
		Table_Next[Capacity - 1] = -1;

		Active_First = -1;
		Active_Last = -1;
		Inactive_First = 0;
		Inactive_Last = Capacity - 1;
	}

	void Delete_Implementation(const int index) {
		if (index > -1 && index < static_cast<int>(Capacity) && Table_IsActive[index] == 1) {
			int prev{ Table_Prev[index] };
			int next{ Table_Next[index] };

			if (index != Active_First) { Table_Next[prev] = Table_Next[index]; }
			else { Active_First = Table_Next[Active_First]; }
			if (index != Active_Last) { Table_Prev[next] = Table_Prev[index]; }
			else { Active_Last = Table_Prev[Active_Last]; }

			if (Inactive_First == -1) {
				Inactive_First = index;
				Table_Prev[index] = -1;
			}
			else {
				Table_Next[Inactive_Last] = index;
				Table_Prev[index] = Inactive_Last;
			}
			Table_Next[index] = -1;
			Inactive_Last = index;

			Table_IsActive[index] = 0;
		}
	}

public:
	template<typename T>
	class Iterator;

	explicit LinkedList() { Initialize(32U); }
	explicit LinkedList(const std::uint32_t _capacity) { Initialize(_capacity); }

	virtual ~LinkedList() {
		assert(Table_Element != nullptr);
		delete[] Table_Element;
		Table_Element = nullptr;

		assert(Table_IsActive != nullptr);
		delete[] Table_IsActive;
		Table_IsActive = nullptr;

		assert(Table_Prev != nullptr);
		delete[] Table_Prev;
		Table_Prev = nullptr;

		assert(Table_Next != nullptr);
		delete[] Table_Next;
		Table_Next = nullptr;
	}

	// Returns pointer to an unused entry if there is any; returns nullptr otherwise.
	T* NewElement() {
		int i{ Inactive_First };
		if (i != -1) {
			if (Active_First == -1) {
				Active_First = i;
			}
			else {
				Table_Prev[i] = Active_Last;
				Table_Next[Active_Last] = i;
			}
			Active_Last = i;
			Inactive_First = Table_Next[Inactive_First];
			if (Inactive_First == -1) { Inactive_Last = -1; }
			Table_Next[i] = -1;

			Table_IsActive[i] = 1;

			return Table_Element + i;
		}
		else {
			return nullptr;
		}
	}

	void Delete(Iterator<T>& _it) { Delete_Implementation(_it.Index_Current); }

	void Clear() {
		std::memset(Table_IsActive, 0, sizeof(bool) * Capacity);
		for (std::uint32_t i{ 0U }; i < Capacity; ++i) {
			Table_Prev[i] = i - 1;
			Table_Next[i] = i + 1;
		}
		Table_Next[Capacity - 1] = -1;

		Active_First = -1;
		Active_Last = -1;
		Inactive_First = 0;
		Inactive_Last = Capacity - 1;
	}

	template<typename T>
	class Iterator {
		friend LinkedList;

	private:
		const LinkedList<T>* Iteratee{ nullptr };
		int Index_Current{ -1 };
		int Index_Next{ -1 };

	public:
		explicit Iterator(const LinkedList<T>* _iteratee) : Iteratee{ _iteratee } {}
		~Iterator() = default;

		constexpr inline void Begin() {
			Index_Current = Iteratee->Active_First;
			Index_Next = Iteratee->Table_Next[Index_Current];
		}
		constexpr inline bool End() const { return (Index_Current == -1); }
		constexpr inline void Next() {
			Index_Current = Index_Next;
			Index_Next = (Index_Next == -1) ? (-1) : (Iteratee->Table_Next[Index_Next]);
		}

		inline T& operator*() {
			assert(Index_Current != -1);
			return Iteratee->Table_Element[Index_Current];
		};
	};
};
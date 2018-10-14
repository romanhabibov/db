#include <chrono>
#include <iostream>

namespace options
{
	constexpr size_t size = 8192;
	constexpr size_t block_size = 256;
	constexpr size_t divider = 32;
}

class Timer
{
public:
	Timer()
		: start_(std::chrono::high_resolution_clock::now())
	{
	}

	~Timer()
	{
		const auto finish = std::chrono::high_resolution_clock::now();
		std::cout << std::chrono::duration_cast<std::chrono::microseconds>(finish - start_).count() << " us" << std::endl;
	}

private:
	const std::chrono::high_resolution_clock::time_point start_;
};


int main()
{
	int** matrix_1 = new int*[options::size];
	for (size_t i = 0; i < options::size; i++)
		matrix_1[i] = new int[options::size];

	int** matrix_2 = new int*[options::size];
	for (size_t i = 0; i < options::size; i++)
		matrix_2[i] = new int[options::size];

	std::cout << "Pointer to pointer." << std::endl;
	{
		Timer t;
		for (size_t i = 0; i < options::size; i++)
			for (size_t j = 0; j < options::size; j++)
				matrix_2[i][j] = matrix_1[j][i];

	}
	{
		Timer t;
		for (size_t row_offset = 0; row_offset < options::divider; row_offset++)
			for (size_t col_offset = 0; col_offset < options::divider; col_offset++)
				for (size_t i = 0; i < options::block_size; i++)
					for (size_t j = 0; j < options::block_size; j++)
						matrix_2[j + row_offset * options::block_size][i + col_offset * options::block_size] = matrix_1[i + col_offset * options::block_size][j + row_offset * options::block_size];
	}

	for (size_t i = 0; i < options::size; i++)
	{
		delete[] matrix_1[i];
		delete[] matrix_2[i];
	}
	delete[] matrix_1;
	delete[] matrix_2;

	int* one_array_matrix_1 = new int[options::size * options::size];
	int* one_array_matrix_2 = new int[options::size * options::size];

	std::cout << "One array." << std::endl;
	{
		Timer t;
		for (size_t i = 0; i < options::size; i++)
			for (size_t j = 0; j < options::size; j++)
				one_array_matrix_2[i * options::size + j] = one_array_matrix_1[j * options::size + i];

	}
	{
		Timer t;
		for (size_t row_offset = 0; row_offset < options::divider; row_offset++)
			for (size_t col_offset = 0; col_offset < options::divider; col_offset++)
				for (size_t i = 0; i < options::block_size; i++)
					for (size_t j = 0; j < options::block_size; j++)
						one_array_matrix_2[(j + row_offset * options::block_size) * options::size + i + col_offset * options::block_size] = one_array_matrix_1[(i + col_offset * options::block_size) * options::size + j + row_offset * options::block_size];
	}

	delete[] one_array_matrix_1;
	delete[] one_array_matrix_2;

	getchar();
}

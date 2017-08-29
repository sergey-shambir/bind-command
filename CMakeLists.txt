# Требуем как можно более высокую версию.
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# Инструкция project не может быть помещена в функцию.
project(bind-command)

# Инструкция enable_testing неявно объявляет опцию BUILD_TESTING,
#  по умолчанию BUILD_TESTING=ON.
enable_testing()

include(cmake/functions.cmake)

# Добавляем исполняемый файл
custom_add_executable_from_dirs(bind-command src src/detail)

# Включаем C++17
custom_enable_cxx17(bind-command)


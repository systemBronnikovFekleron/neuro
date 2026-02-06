#!/bin/bash
# Тестовый скрипт для проверки работы приложения

echo "=== Запуск тестирования BronnikovExerciseApp ==="
echo ""
echo "Дата: $(date)"
echo "Устройство должно быть включено и рядом!"
echo ""
echo "Запуск приложения..."
echo "Нажмите Ctrl+C для выхода"
echo ""

cd "$(dirname "$0")/build/Release"
./BronnikovExerciseApp 2>&1 | tee ../../logs/test_$(date +%Y%m%d_%H%M%S).log

echo ""
echo "=== Тест завершен ==="
echo "Лог сохранен в BronnikovExerciseApp/logs/"

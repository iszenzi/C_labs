if [ "$#" -ne 2 ]; then
    echo "Ошибка: нужно 2 аргумента"
    echo "Использование: $0 <суффикс_для_удаления> <суффикс_триггер>"
    exit 1
fi

TARGET_SUFFIX=$1
TRIGGER_SUFFIX=$2

echo "Запуск очистки"
echo "Цель для удаления: *$TARGET_SUFFIX"
echo "Триггер наличия:   *$TRIGGER_SUFFIX"
echo "-----------------------------------"

for file in *"$TARGET_SUFFIX"; do

    if [ ! -e "$file" ]; then
        echo "Файлы с суффиксом '$TARGET_SUFFIX' не найдены"
        break
    fi

    base_name="${file%"$TARGET_SUFFIX"}"

    trigger_file="${base_name}${TRIGGER_SUFFIX}"

    if [ -f "$trigger_file" ]; then
        echo "Найдена пара: существует '$trigger_file'. Удаляю '$file'..."
        rm "$file"
    else
        echo "Пропуск: для файла '$file' пара '$trigger_file' не найдена."
    fi
done

echo "-----------------------------------"
echo "Работа успешно завершена"
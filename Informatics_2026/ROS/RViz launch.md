# ROS 2: source, colcon, ros2 launch

## source — что это
- `source /opt/ros/jazzy/setup.bash` добавляет ROS-команды и пакеты в текущий shell.
- Без source команда `ros2` не найдётся, `ros2 launch` не увидит пакеты [citation:1][citation:7].
- Нужен в каждом новом терминале (в контейнере — каждом новом `bash`).

## Автоматизация source
- В Dockerfile уже есть:
  `RUN echo "source /opt/ros/jazzy/setup.bash" >> ~/.bashrc`
- Значит, при входе в bash ROS-окружение подхватится само.
- Для своего workspace можно добавить:
  `[ -f /workspace/install/setup.bash ] && source /workspace/install/setup.bash` [citation:10]

## colcon — что это
- Отдельный сборщик пакетов ROS 2 (не Docker).
- `colcon build` собирает все пакеты из `src/` в `build/` и `install/` [citation:2].
- Запускать из корня workspace: `cd /workspace && colcon build`.

## --symlink-install — что значит
- Вместо копирования файлов из `src/` в `install/` создаёт симлинки (ссылки).
- Меняешь launch/URDF в `src/` → изменения сразу видны в `install/` без пересборки.
- Для домашки удобно: правишь `display.launch.py`, не пересобирая.

## ros2 launch — что это
- Запускает launch-файл из установленного пакета.
- Ищет файл в `install/<package>/share/<package>/launch/`, не в `src/`.
- Поэтому после правок в `src/` нужен `colcon build` (или `--symlink-install`).

## Полная последовательность (в контейнере)
```bash
# ROS уже подхвачен через .bashrc
cd /workspace
colcon build --symlink-install
source install/setup.bash
ros2 launch hexapod_description display.launch.py
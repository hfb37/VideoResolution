#include "VideoResolution.h"
#include <regex>
#include <string>
#include <opencv2/videoio.hpp>


// Класс для работы непосредственно с видео-файлом.
class VideoFile {
public:
    // Конструктор принимает путь к видео-файлу.
    explicit VideoFile(const std::string& filePath) : filePath_(filePath) {}

    // Метод для получения разрешения видео.
    std::pair<int, int> getResolution() {
        cv::VideoCapture cap(filePath_); // Открытие видео-файла.
        if (!cap.isOpened()) {
            // Бросает исключение, если файл не удалось открыть.
            throw std::runtime_error("Failed to open video file: " + filePath_);
        }

        // Получение ширины и высоты из метаданных видео.
        int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
        int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
        return {width, height};
    }

private:
    std::string filePath_; // Путь к файлу.
};

// Прокси-класс, который добавляет проверку имени файла и ленивую инициализацию.
class VideoFileProxy {
public:
    // Конструктор принимает путь к видео-файлу.
    explicit VideoFileProxy(const std::string& filePath) : filePath_(filePath), realVideoFile_(nullptr) {}

    // Деструктор освобождает память, выделенную под реальный объект.
    ~VideoFileProxy() {
        delete realVideoFile_;
    }

    // Метод для получения разрешения видео с предварительной проверкой имени файла.
    std::pair<int, int> getResolution() {
        // Регулярное выражение для проверки шаблона "_WWWxHHH".
        std::regex pattern(".*_(\\d+)x(\\d+).*");
        std::smatch match;

        if (std::regex_match(filePath_, match, pattern)) {
            try {
                // Если формат имени файла корректен, извлекаем ширину и высоту.
                int width = std::stoi(match[1].str());
                int height = std::stoi(match[2].str());
                return {width, height};
            } catch (const std::exception&) {
                // Игнорируем ошибки преобразования и переходим к открытию файла.
            }
        }

        // Если формат имени файла не соответствует, создаём объект VideoFile.
        if (!realVideoFile_) {
            realVideoFile_ = new VideoFile(filePath_);
        }
        return realVideoFile_->getResolution();
    }

private:
    std::string filePath_; // Путь к файлу.
    VideoFile* realVideoFile_; // Указатель на реальный объект.
};

std::pair<int, int> getResolution(const std::string &filePath) {
    VideoFileProxy proxy1(filePath);
    auto res = proxy1.getResolution();
    return res;
}

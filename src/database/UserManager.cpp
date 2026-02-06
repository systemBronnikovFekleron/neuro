#include "UserManager.h"
#include "SessionDatabase.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

namespace Bronnikov {

UserManager::UserManager(SessionDatabase& db, const std::string& data_root)
    : m_database(db)
    , m_data_root(data_root)
{
    // Создаем корневую директорию для пользователей
    fs::path users_root = fs::path(m_data_root) / "Users";
    if (!fs::exists(users_root)) {
        fs::create_directories(users_root);
    }
}

int UserManager::loadUsers() {
    m_users.clear();

    // Получаем список всех пользователей из БД
    // TODO: В SessionDatabase нет метода для получения всех пользователей,
    // поэтому временно загрузим только "default"
    // В будущем нужно добавить метод getAllUsers() в SessionDatabase

    // Загружаем default пользователя
    auto default_user = std::make_unique<UserProfile>("default");
    if (default_user->load(m_database)) {
        std::cout << "[UserManager] Загружен пользователь: default" << std::endl;
    } else {
        // Создаем профиль для default пользователя
        default_user->setName("Default User");
        default_user->save(m_database);
        std::cout << "[UserManager] Создан новый профиль: default" << std::endl;
    }

    m_users["default"] = std::move(default_user);
    m_current_user_id = "default";

    return static_cast<int>(m_users.size());
}

bool UserManager::createUser(const std::string& user_id, const std::string& name) {
    if (user_id.empty() || userExists(user_id)) {
        std::cerr << "[UserManager] Пользователь уже существует или ID пуст: " << user_id << std::endl;
        return false;
    }

    // Создаем новый профиль
    auto user = std::make_unique<UserProfile>(user_id);
    user->setName(name.empty() ? user_id : name);

    // Сохраняем в БД
    if (!user->save(m_database)) {
        std::cerr << "[UserManager] Не удалось сохранить профиль: " << user_id << std::endl;
        return false;
    }

    // Создаем файловую структуру
    if (!createUserDirectories(user_id)) {
        std::cerr << "[UserManager] Не удалось создать директории для: " << user_id << std::endl;
        return false;
    }

    std::cout << "[UserManager] ✓ Создан пользователь: " << user_id
              << " (" << user->getName() << ")" << std::endl;

    m_users[user_id] = std::move(user);
    return true;
}

bool UserManager::deleteUser(const std::string& user_id) {
    if (!userExists(user_id)) {
        return false;
    }

    // TODO: Добавить метод deleteUserProfile() в SessionDatabase
    // Пока просто удаляем из памяти
    m_users.erase(user_id);

    // Если удаляем текущего пользователя, переключаемся на default
    if (m_current_user_id == user_id) {
        m_current_user_id = "default";
    }

    std::cout << "[UserManager] Удален пользователь: " << user_id << std::endl;
    return true;
}

bool UserManager::setCurrentUser(const std::string& user_id) {
    if (!userExists(user_id)) {
        std::cerr << "[UserManager] Пользователь не найден: " << user_id << std::endl;
        return false;
    }

    m_current_user_id = user_id;
    std::cout << "[UserManager] Текущий пользователь: " << user_id << std::endl;
    return true;
}

UserProfile* UserManager::getCurrentUser() {
    auto it = m_users.find(m_current_user_id);
    return (it != m_users.end()) ? it->second.get() : nullptr;
}

const UserProfile* UserManager::getCurrentUser() const {
    auto it = m_users.find(m_current_user_id);
    return (it != m_users.end()) ? it->second.get() : nullptr;
}

UserProfile* UserManager::getUser(const std::string& user_id) {
    auto it = m_users.find(user_id);
    return (it != m_users.end()) ? it->second.get() : nullptr;
}

const UserProfile* UserManager::getUser(const std::string& user_id) const {
    auto it = m_users.find(user_id);
    return (it != m_users.end()) ? it->second.get() : nullptr;
}

std::vector<std::string> UserManager::getUserList() const {
    std::vector<std::string> list;
    list.reserve(m_users.size());

    for (const auto& [user_id, profile] : m_users) {
        list.push_back(user_id);
    }

    return list;
}

bool UserManager::saveUserProfile(const std::string& user_id) {
    auto user = getUser(user_id);
    if (!user) {
        return false;
    }

    return user->save(m_database);
}

int UserManager::saveAllProfiles() {
    int saved = 0;

    for (const auto& [user_id, profile] : m_users) {
        if (profile->save(m_database)) {
            ++saved;
        }
    }

    std::cout << "[UserManager] Сохранено профилей: " << saved << "/" << m_users.size() << std::endl;
    return saved;
}

std::string UserManager::getUserDataPath(const std::string& user_id) const {
    fs::path user_path = fs::path(m_data_root) / "Users" / user_id;
    return user_path.string();
}

std::string UserManager::getUserSessionsPath(const std::string& user_id) const {
    fs::path sessions_path = fs::path(m_data_root) / "Users" / user_id / "sessions";
    return sessions_path.string();
}

std::string UserManager::getUserResearchPath(const std::string& user_id) const {
    fs::path research_path = fs::path(m_data_root) / "Users" / user_id / "research";
    return research_path.string();
}

bool UserManager::createUserDirectories(const std::string& user_id) {
    try {
        // Создаем основную директорию пользователя
        fs::path user_path = fs::path(m_data_root) / "Users" / user_id;
        if (!fs::exists(user_path)) {
            fs::create_directories(user_path);
        }

        // Создаем поддиректории
        fs::path sessions_path = user_path / "sessions";
        if (!fs::exists(sessions_path)) {
            fs::create_directories(sessions_path);
        }

        fs::path research_path = user_path / "research";
        if (!fs::exists(research_path)) {
            fs::create_directories(research_path);
        }

        std::cout << "[UserManager] ✓ Созданы директории для пользователя: " << user_id << std::endl;
        std::cout << "  - " << user_path << std::endl;
        std::cout << "  - " << sessions_path << std::endl;
        std::cout << "  - " << research_path << std::endl;

        return true;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "[UserManager] Ошибка создания директорий: " << e.what() << std::endl;
        return false;
    }
}

bool UserManager::userExists(const std::string& user_id) const {
    return m_users.find(user_id) != m_users.end();
}

} // namespace Bronnikov

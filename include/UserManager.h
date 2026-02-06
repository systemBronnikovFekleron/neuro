#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "UserProfile.h"

namespace Bronnikov {

// Forward declaration
class SessionDatabase;

/**
 * @brief Менеджер пользователей
 *
 * Управляет списком пользователей, создание новых профилей,
 * переключение между пользователями, и организацию файловой структуры.
 */
class UserManager {
public:
    UserManager(SessionDatabase& db, const std::string& data_root = "data");
    ~UserManager() = default;

    // Запрет копирования
    UserManager(const UserManager&) = delete;
    UserManager& operator=(const UserManager&) = delete;

    /**
     * @brief Загрузить список всех пользователей из БД
     * @return Количество загруженных пользователей
     */
    int loadUsers();

    /**
     * @brief Создать нового пользователя
     * @param user_id Уникальный ID пользователя
     * @param name Отображаемое имя
     * @return true если пользователь создан
     */
    bool createUser(const std::string& user_id, const std::string& name = "");

    /**
     * @brief Удалить пользователя (только из БД, не удаляет файлы)
     * @param user_id ID пользователя
     * @return true если удален
     */
    bool deleteUser(const std::string& user_id);

    /**
     * @brief Установить текущего пользователя
     * @param user_id ID пользователя
     * @return true если пользователь существует и установлен
     */
    bool setCurrentUser(const std::string& user_id);

    /**
     * @brief Получить текущего пользователя
     * @return Указатель на текущий профиль (nullptr если не установлен)
     */
    UserProfile* getCurrentUser();
    const UserProfile* getCurrentUser() const;

    /**
     * @brief Получить пользователя по ID
     * @param user_id ID пользователя
     * @return Указатель на профиль (nullptr если не найден)
     */
    UserProfile* getUser(const std::string& user_id);
    const UserProfile* getUser(const std::string& user_id) const;

    /**
     * @brief Получить список всех пользователей
     * @return Вектор ID пользователей
     */
    std::vector<std::string> getUserList() const;

    /**
     * @brief Сохранить профиль пользователя
     * @param user_id ID пользователя
     * @return true если успешно
     */
    bool saveUserProfile(const std::string& user_id);

    /**
     * @brief Сохранить все профили
     * @return Количество сохраненных профилей
     */
    int saveAllProfiles();

    /**
     * @brief Получить путь к директории данных пользователя
     * @param user_id ID пользователя
     * @return Путь вида "data/Users/[user_id]/"
     */
    std::string getUserDataPath(const std::string& user_id) const;

    /**
     * @brief Получить путь к директории сессий пользователя
     * @param user_id ID пользователя
     * @return Путь вида "data/Users/[user_id]/sessions/"
     */
    std::string getUserSessionsPath(const std::string& user_id) const;

    /**
     * @brief Получить путь к директории research данных пользователя
     * @param user_id ID пользователя
     * @return Путь вида "data/Users/[user_id]/research/"
     */
    std::string getUserResearchPath(const std::string& user_id) const;

    /**
     * @brief Создать файловую структуру для пользователя
     * @param user_id ID пользователя
     * @return true если директории созданы
     */
    bool createUserDirectories(const std::string& user_id);

private:
    SessionDatabase& m_database;
    std::string m_data_root;

    std::map<std::string, std::unique_ptr<UserProfile>> m_users;
    std::string m_current_user_id;

    /**
     * @brief Проверить существование пользователя
     */
    bool userExists(const std::string& user_id) const;
};

} // namespace Bronnikov

#include <iostream>
#include <memory>
#include <string>
#include <cassert>

class IDBConnection {
public:
    virtual ~IDBConnection() = default;
    virtual bool open() = 0;
    virtual bool close() = 0;
    virtual std::string execQuery(const std::string& query) = 0;
};

class MockDBConnection : public IDBConnection {
public:
    bool open() override {
        std::cout << "MockDB: open() called\n";
        return openResult;
    }

    bool close() override {
        std::cout << "MockDB: close() called\n";
        return closeResult;
    }

    std::string execQuery(const std::string& query) override {
        std::cout << "MockDB: execQuery() called with: " << query << "\n";
        return queryResult;
    }

    bool openResult = true;
    bool closeResult = true;
    std::string queryResult = "mock_result";
};

class ClassThatUsesDB {
public:
    ClassThatUsesDB(std::shared_ptr<IDBConnection> connection) 
        : dbConnection(connection) {}

    bool openConnection() {
        return dbConnection->open();
    }

    std::string useConnection(const std::string& query) {
        return dbConnection->execQuery(query);
    }

    bool closeConnection() {
        return dbConnection->close();
    }

private:
    std::shared_ptr<IDBConnection> dbConnection;
};

void runTests() {
    auto mockConn = std::make_shared<MockDBConnection>();
    ClassThatUsesDB dbUser(mockConn);

    mockConn->openResult = true;
    assert(dbUser.openConnection() == true);

    mockConn->closeResult = false;
    assert(dbUser.closeConnection() == false);

    mockConn->queryResult = "test_data";
    assert(dbUser.useConnection("SELECT * FROM table") == "test_data");

    std::cout << "All tests passed!\n";
}
int main() {
    runTests();

    class RealDBConnection : public IDBConnection {
        bool open() override { return true; }
        bool close() override { return true; }
        std::string execQuery(const std::string& q) override { return "real_data"; }
    };

    auto realConn = std::make_shared<RealDBConnection>();
    ClassThatUsesDB realDBUser(realConn);

    std::cout << "Real DB query result: " << realDBUser.useConnection("SELECT 1") << "\n";

    return 0;
}
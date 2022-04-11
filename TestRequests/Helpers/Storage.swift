//
//  Storage.swift
//  TestRequests
//
//  Created by Nazar on 28.03.2022.
//

import Foundation

// options: core data, sqlite, realm, json file
final class Storage {
    private var database: OpaquePointer

    init?() {
        guard let path = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first?.appendingPathComponent("database", isDirectory: false)
        else {
            print("can't create db path")
            return nil
        }
        print("path", path)

        guard let db = openSQLiteDB(path.absoluteString.cString(using: .utf8))
        else {
            print("openSQLiteDB returned nil")
            return nil
        }

        database = db
        ensureTableExistsSQLiteDB(database)
    }

    var nextEvent: EventObject? {
        selectNextEventSQLiteDB(database)?.pointee
    }

    func insert(event: Event) -> sqlite3_int64? {
        insertSQLiteDB(database, event.subject.cString(using: .utf8), event.subject.cString(using: .utf8))?.pointee
    }

    func delete(id: sqlite3_int64) {
        deleteSQLiteDB(database, id)
    }

    deinit {
        closeSQLiteDB(database)
    }
}

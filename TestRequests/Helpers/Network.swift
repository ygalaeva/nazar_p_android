//
//  Network.swift
//  TestRequests
//
//  Created by Nazar on 29.03.2022.
//

import Foundation

final class Network {
    // quick dirty way
    func request(event: Event, completion: @escaping (Bool) -> Void) throws {
        let urlString = "https://test22.free.beeceptor.com/my/api/path"
        guard let url = URL(string: urlString) else {
            print(#function, "can't create URL", urlString)
            completion(false)
            return
        }
        var request = URLRequest(url: url, cachePolicy: .useProtocolCachePolicy, timeoutInterval: 10)
        request.addValue("application/json", forHTTPHeaderField: "Content-Type")
        request.httpMethod = "POST"
        request.httpBody = try JSONEncoder().encode(event)
        URLSession.shared.dataTask(with: request) { data, response, error in
            guard
                error == nil,
                let statusCode = (response as? HTTPURLResponse)?.statusCode,
                statusCode == 200
            else {
                completion(false)
                return
            }

            completion(true)
        }.resume()
    }
}

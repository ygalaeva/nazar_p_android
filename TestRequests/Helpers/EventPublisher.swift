//
//  EventPublisher.swift
//  TestRequests
//
//  Created by Nazar on 29.03.2022.
//

import Foundation

struct Event: Encodable {
    let subject: String
    let payload: String
}

protocol EventPublisherProtocol {
    func publish(event: Event, queue: DispatchQueue)
}

final class EventPublisher: EventPublisherProtocol {
    private let storage: Storage
    private let network: Network

    init?() {
        guard let storageUnwrapped = Storage() else { return nil }
        storage = storageUnwrapped
        network = Network()
    }

    func publish(event: Event, queue: DispatchQueue) {
        // 1. save in local storage
        let identityOptional = queue.sync { storage.insert(event: event) }
        guard let identity = identityOptional else { return }

        // 2. publish on server
        do {
            try network.request(event: event) { [weak self] result in
                if result {
                    self?.storage.delete(id: identity)
                }
            }
        } catch {
            print(#function, "request failed")
        }
    }

    // publish saved events
    func rePublish(queue: DispatchQueue) {
        guard
            let eventObject = storage.nextEvent,
            let subject = eventObject.subject,
            let payload = eventObject.payload
        else { return }

        let event = Event(subject: String(cString: subject), payload: String(cString: payload))

        do {
            try network.request(event: event) { [weak self] result in
                if result {
                    self?.storage.delete(id: eventObject.id)
                }
                self?.rePublish(queue: queue)
            }
        } catch {
            print(#function, "request failed")
            rePublish(queue: queue)
        }
    }
}

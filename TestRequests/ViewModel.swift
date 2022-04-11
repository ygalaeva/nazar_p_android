//
//  ViewModel.swift
//  TestRequests
//
//  Created by Nazar on 29.03.2022.
//

import Foundation

final class ViewModel {
    private let eventPublisher: EventPublisher
    private let queue = DispatchQueue(label: "EventPublisherQueue")

    init?() {
        guard let eventPublisherUnwrapped = EventPublisher() else { return nil }
        eventPublisher = eventPublisherUnwrapped
    }

    func publish(event: Event) {
        eventPublisher.publish(event: event, queue: queue)
    }

    func rePublish() {
        eventPublisher.rePublish(queue: queue)
    }
}

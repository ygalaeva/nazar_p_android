//
//  ViewController.swift
//  TestRequests
//
//  Created by Nazar on 28.03.2022.
//

import UIKit

class ViewController: UIViewController {

    private let viewModel = ViewModel()

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.

        configureUI()
    }

    private func configureUI() {
        let publishButton = UIButton()
        publishButton.setTitle("Publish", for: .normal)
        publishButton.setTitleColor(.blue, for: .normal)
        publishButton.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(publishButton)
        NSLayoutConstraint.activate([
            publishButton.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor),
            publishButton.centerXAnchor.constraint(equalTo: view.centerXAnchor)
        ])
        publishButton.addTarget(self, action: #selector(publishButtonAction), for: .touchUpInside)

        let rePublishButton = UIButton()
        rePublishButton.setTitle("rePublish", for: .normal)
        rePublishButton.setTitleColor(.blue, for: .normal)
        rePublishButton.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(rePublishButton)
        NSLayoutConstraint.activate([
            rePublishButton.topAnchor.constraint(equalTo: publishButton.bottomAnchor, constant: 20),
            rePublishButton.centerXAnchor.constraint(equalTo: view.centerXAnchor)
        ])
        rePublishButton.addTarget(self, action: #selector(rePublishButtonAction), for: .touchUpInside)
    }

    @objc private func publishButtonAction() {
        viewModel?.publish(event: Event(subject: "CurrentTime", payload: "\(Date())"))
    }

    @objc private func rePublishButtonAction() {
        viewModel?.rePublish()
    }
}


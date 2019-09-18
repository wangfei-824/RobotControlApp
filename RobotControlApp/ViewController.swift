//
//  ViewController.swift
//  RobotCtl
//
//  Created by Lucky on 19/8/12.
//  Copyright © 2019年 Felix. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    @IBOutlet weak var connectBtn: UIButton!
    @IBOutlet weak var stopBtn: UIButton!
    @IBOutlet weak var return2OriBtn: UIButton!
    @IBOutlet weak var cameraBtn: UIButton!
    
    private var speedScale = 0.5
    
    func increaseSpeedScale() {
        self.speedScale += 0.05
        if self.speedScale > 1 {
            self.speedScale = 1
        }
        self.messageLabel.text = "speed scale: \(self.speedScale)"
    }
    
    func decreaseSpeedScale() {
        self.speedScale -= 0.05
        if self.speedScale < 0.1 {
            self.speedScale = 0.1
        }
        self.messageLabel.text = "speed scale: \(self.speedScale)"
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        
        let swipeUpRecognizer = UISwipeGestureRecognizer(
            target: self, action: #selector(increaseSpeedScale))
        swipeUpRecognizer.direction = .up
        self.view.addGestureRecognizer(swipeUpRecognizer)
        
        let swipeDownRecognizer = UISwipeGestureRecognizer(
            target: self, action: #selector(decreaseSpeedScale))
        swipeDownRecognizer.direction = .down
        self.view.addGestureRecognizer(swipeDownRecognizer)
    }
    
    @IBOutlet weak var messageLabel: UILabel!
    
    private let stateMessage = [
        "wifiOk" : "Wifi connected!",
        "wifiNotOk" : "Wifi disconnected!"]

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    @IBOutlet weak var joystickView: MyJoystickView! {
        didSet {
            let handler = #selector(changeDirectionAndAngularSpeed(byReactingTo:))
            let panRecognizer = UIPanGestureRecognizer(target: self, action: handler)
            joystickView.addGestureRecognizer(panRecognizer)
        }
    }
    
    private var isStartedToSendVelocity = false
    
    func changeDirectionAndAngularSpeed(byReactingTo recognizer : UIPanGestureRecognizer) {
        switch recognizer.state {
        case .began:
            self.isStartedToSendVelocity = true
            let conqueue = DispatchQueue(label: "rosSendVelocity", attributes: .concurrent)
            conqueue.async {
                while self.isStartedToSendVelocity {
                    let position = recognizer.translation(in: self.joystickView)
                    let shiftPen = self.joystickView.getBallPositionPercentage(shiftX: position.x, shiftY: position.y)
                    if shiftPen.0 >= -1 {
                        var robLinearSpeed = -shiftPen.1
                        var robAngularSpeed : Double
                        if (shiftPen.0 < 0 && robLinearSpeed > 0) ||
                            (shiftPen.0 > 0 && robLinearSpeed < 0){
                            robAngularSpeed = 1 - fabs(atan(shiftPen.1/shiftPen.0)) * 2 / M_PI
                        } else {
                            robAngularSpeed = -1 + fabs(atan(shiftPen.1/shiftPen.0)) * 2 / M_PI
                        }
                        
                        robLinearSpeed *= self.speedScale
                        robAngularSpeed *= self.speedScale
                        
                        rosMainSendCmdVelocity(robLinearSpeed, robAngularSpeed)
                        
//                        self.messageLabel.text = "speed: \(robLinearSpeed), \(robAngularSpeed)"
                    }
                    Thread.sleep(forTimeInterval: 0.2)
                }
                rosMainSendCmdVelocity(0.0, 0.0)
                
//                self.messageLabel.text = "speed: \(0.0), \(0.0)"
            }
        case .changed:
            let position = recognizer.translation(in: joystickView)
            joystickView.changeBallPosition(shiftX: position.x, shiftY: position.y)
        default:
            joystickView.changeBallPosition(shiftX: 0, shiftY: 0)
            self.isStartedToSendVelocity = false
            rosMainSendCmdVelocity(0.0, 0.0)
            
//            self.messageLabel.text = "speed: \(0.0), \(0.0)"
        }
    }

    @IBAction func connectRosServer(_ sender: UIButton) {
        if rosMainIsConnected() {
            rosMainClose()
            
            joystickView.isEnabled = false
            stopBtn.isEnabled = false
            return2OriBtn.isEnabled = false
            cameraBtn.isEnabled = false
            connectBtn.setBackgroundImage(#imageLiteral(resourceName: "0"), for: UIControlState.normal)
            
            messageLabel.text = stateMessage["wifiNotOk"]
        } else {
            let host = "10.0.0.1"
            let c_host = (host as NSString).utf8String
            let buffer = UnsafeMutablePointer<Int8>(mutating: c_host)
            rosMainInit(buffer)
            
            if rosMainIsConnected() {
                let conqueue = DispatchQueue(label: "rosloop", attributes: .concurrent)
                conqueue.async {
                    while rosMainIsConnected() {
                        rosMainLoop()
                        Thread.sleep(forTimeInterval: 0.1)
                    }
                }
        
                joystickView.isEnabled = true
                stopBtn.isEnabled = true
                return2OriBtn.isEnabled = true
                cameraBtn.isEnabled = true
                connectBtn.setBackgroundImage(#imageLiteral(resourceName: "4"), for: UIControlState.normal)
                
                messageLabel.text = stateMessage["wifiOk"]
            }
        }
    }
    
    func poseCallback(x:Double, y:Double, z:Double, roll:Double, pitch:Double, yaw:Double) {
        
    }
}


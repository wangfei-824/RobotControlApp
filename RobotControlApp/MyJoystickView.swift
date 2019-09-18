//
//  MyJoystickView.swift
//  RobotCtl
//
//  Created by Lucky on 19/8/12.
//  Copyright © 2019年 Felix. All rights reserved.
//

import UIKit

class MyJoystickView: UIView {

    /*
    // Only override draw() if you perform custom drawing.
    // An empty implementation adversely affects performance during animation.
    override func draw(_ rect: CGRect) {
        // Drawing code
    }
    */
    @IBOutlet weak var ball: UIImageView!
    
    @IBOutlet var view: UIView!
    
    @IBOutlet weak var backImage: UIImageView!
    
    var isEnabled : Bool = false 

    override init(frame: CGRect) {
        super.init(frame: frame)
        initViewFromXib()
    }
    
    required init?(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)
        initViewFromXib()
    }
    
    convenience init() {
        self.init(frame: CGRect.zero)
    }
    
    private func initViewFromXib() {
        let bundle = Bundle(for: type(of: self))
        let nib = UINib(nibName: "MyJoystickView", bundle: bundle)
        self.view = nib.instantiate(withOwner: self, options: nil).first as! UIView
        self.view.frame = self.bounds
        self.addSubview(self.view)
    }
    
    override func layoutSubviews() {
        self.view.frame = self.bounds
    }
    
    func changeBallPosition(shiftX: CGFloat, shiftY: CGFloat){
        if self.isEnabled {
            let radius = min(backImage.bounds.midX, backImage.bounds.midY)
            let len = hypot(shiftX, shiftY)
        
            let ballShiftX : CGFloat
            let ballShiftY : CGFloat
            if len > radius {
                ballShiftX = shiftX * radius / len
                ballShiftY = shiftY * radius / len
            } else {
                ballShiftX = shiftX
                ballShiftY = shiftY
            }
        
            ball.center.x = backImage.center.x + ballShiftX
            ball.center.y = backImage.center.y + ballShiftY
        }
    }
    
    func getBallPositionPercentage(shiftX: CGFloat, shiftY: CGFloat) -> (Double, Double) {
        if self.isEnabled {
            let radius = min(backImage.bounds.midX, backImage.bounds.midY)
            let len = hypot(shiftX, shiftY)
            
            let ballShiftX : CGFloat
            let ballShiftY : CGFloat
            if len > radius {
                ballShiftX = shiftX * radius / len
                ballShiftY = shiftY * radius / len
            } else {
                ballShiftX = shiftX
                ballShiftY = shiftY
            }
            
            let shiftXper = Double(ballShiftX / radius)
            let shiftYper = Double(ballShiftY / radius)
            
            return (shiftXper, shiftYper)
        } else {
            return (-2, -2)
        }
    }
    
}

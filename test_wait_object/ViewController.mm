//
//  ViewController.m
//  test_wait_object
//
//  Created by zuoyu on 2020/4/29.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#import "ViewController.h"
#include "wait_timeout.h"
#include "simple_memory.h"
#include "timeout_object.hpp"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
//    test_wait_timeout();
//    test_simple_memory();
    test_timeout_object_function();
}


@end

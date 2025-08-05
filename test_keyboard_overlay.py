#!/usr/bin/env python3
"""
Test script for the Keyboard Overlay Plugin
This script demonstrates how the keyboard overlay works with simulated input.
"""

import sys
import time
import threading
from keyboard_overlay import KeyboardOverlayPlugin, SimpleControllerState

def simulate_keyboard_input():
    """Simulate keyboard input for testing the overlay"""
    
    # Create the plugin
    plugin = KeyboardOverlayPlugin()
    
    # Initialize the plugin
    plugin.initialize()
    
    # Wait a bit for the overlay to appear
    time.sleep(1)
    
    # Simulate different keyboard inputs
    test_scenarios = [
        # Scenario 1: Forward movement
        {
            'name': 'Forward (W)',
            'controller': SimpleControllerState(throttle=1.0, steer=0.0, jump=False, boost=False, handbrake=False)
        },
        # Scenario 2: Backward movement
        {
            'name': 'Backward (S)',
            'controller': SimpleControllerState(throttle=-1.0, steer=0.0, jump=False, boost=False, handbrake=False)
        },
        # Scenario 3: Left turn
        {
            'name': 'Left Turn (A)',
            'controller': SimpleControllerState(throttle=0.5, steer=-1.0, jump=False, boost=False, handbrake=False)
        },
        # Scenario 4: Right turn
        {
            'name': 'Right Turn (D)',
            'controller': SimpleControllerState(throttle=0.5, steer=1.0, jump=False, boost=False, handbrake=False)
        },
        # Scenario 5: Jump
        {
            'name': 'Jump (Space)',
            'controller': SimpleControllerState(throttle=0.0, steer=0.0, jump=True, boost=False, handbrake=False)
        },
        # Scenario 6: Boost
        {
            'name': 'Boost (Shift)',
            'controller': SimpleControllerState(throttle=1.0, steer=0.0, jump=False, boost=True, handbrake=False)
        },
        # Scenario 7: Handbrake
        {
            'name': 'Handbrake (Ctrl)',
            'controller': SimpleControllerState(throttle=0.0, steer=-0.5, jump=False, boost=False, handbrake=True)
        },
        # Scenario 8: Aerial control
        {
            'name': 'Aerial Control (Q/E/Arrows)',
            'controller': SimpleControllerState(throttle=0.0, steer=0.0, pitch=0.8, yaw=-0.6, roll=0.4, jump=True, boost=True, handbrake=False)
        },
        # Scenario 9: Complex movement
        {
            'name': 'Complex Movement',
            'controller': SimpleControllerState(throttle=0.8, steer=0.3, pitch=0.2, yaw=0.1, roll=-0.1, jump=True, boost=True, handbrake=True)
        },
        # Scenario 10: No input
        {
            'name': 'No Input',
            'controller': SimpleControllerState(throttle=0.0, steer=0.0, pitch=0.0, yaw=0.0, roll=0.0, jump=False, boost=False, handbrake=False)
        }
    ]
    
    print("Starting keyboard overlay test...")
    print("The overlay should appear in the upper right corner of your screen.")
    print("Press Ctrl+C to stop the test.\n")
    
    try:
        scenario_index = 0
        while True:
            # Get current scenario
            scenario = test_scenarios[scenario_index]
            controller = scenario['controller']
            
            print(f"Testing: {scenario['name']}")
            
            # Update the overlay with this controller state
            plugin.controller_filter(controller)
            
            # Wait 2 seconds before next scenario
            time.sleep(2)
            
            # Move to next scenario
            scenario_index = (scenario_index + 1) % len(test_scenarios)
            
    except KeyboardInterrupt:
        print("\nTest stopped by user.")
    finally:
        # Clean up
        plugin.shutdown()
        print("Keyboard overlay test completed.")

if __name__ == "__main__":
    # Check if PySide6 is available
    try:
        from PySide6.QtWidgets import QApplication
        app = QApplication.instance()
        if app is None:
            app = QApplication(sys.argv)
    except ImportError:
        print("ERROR: PySide6 is not installed.")
        print("Please install it with: pip install PySide6")
        sys.exit(1)
    
    # Run the test
    simulate_keyboard_input()
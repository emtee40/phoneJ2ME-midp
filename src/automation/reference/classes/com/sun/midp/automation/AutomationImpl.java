/*
 *   
 *
 * Copyright  1990-2007 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */


package com.sun.midp.automation;
import com.sun.midp.events.*;

final class AutomationImpl extends Automation {
    private static AutomationImpl instance = null;
    private final static Object lock = new Object();

    private EventQueue eventQueue;
    private AutoEventFactoryImpl eventFactory;
    private int[] foregroundIsolateAndDisplay;
    
    private AutomationImpl(EventQueue eventQueue) {
        this.eventQueue = eventQueue;
        this.eventFactory = AutoEventFactoryImpl.getInstance();
        this.foregroundIsolateAndDisplay = new int[2];
    }

    final static Automation getInstanceImpl() 
        throws IllegalStateException {

        synchronized (lock) {
            AutomationInitializer.guaranteeAutomationInitialized();

            if (instance == null) {
                instance = new AutomationImpl(
                        AutomationInitializer.getEventQueue());
            }
        }
        
        return instance;
    }

    /**
     * Gets instance of AutoSuiteStorage class.
     *
     * @return instance of AutoSuiteStorage class
     * @throws IllegalStateException if Automation API hasn't been
     * initialized or is not permitted to use
     */    
    public AutoSuiteStorage getStorage() 
        throws IllegalStateException {

            return AutoSuiteStorageImpl.getInstance();
    }

    public AutoEventFactory getEventFactory() 
        throws IllegalStateException {

        return AutoEventFactoryImpl.getInstance();
    }

    public void injectEvent(AutoEvent event) 
        throws IllegalArgumentException {

        if (event == null) {
            throw new IllegalArgumentException("Event is null");
        }

        AutoEventImplBase eventBase = (AutoEventImplBase)event;
        NativeEvent nativeEvent = eventBase.toNativeEvent();
        if (nativeEvent == null) {
            throw new IllegalArgumentException(
                    "Can't inject this type of event: " + 
                    eventBase.getType().getName());
        }

        int forgeroundIsolateId;
        int forgeroundDisplayId;
        synchronized (foregroundIsolateAndDisplay) {
            getForegroundIsolateAndDisplay(foregroundIsolateAndDisplay);
            forgeroundIsolateId = foregroundIsolateAndDisplay[0];
            forgeroundDisplayId = foregroundIsolateAndDisplay[1];
        }

        nativeEvent.intParam4 = forgeroundDisplayId;
        eventQueue.sendNativeEventToIsolate(nativeEvent,forgeroundIsolateId);
    }
    
    public void injectKeyEvent(AutoKeyCode keyCode, AutoKeyState keyState) {
        AutoEvent e = eventFactory.createKeyEvent(keyCode, keyState);
        injectEvent(e);
    }
    
    public void injectKeyEvent(char keyChar, AutoKeyState keyState) {
        AutoEvent e = eventFactory.createKeyEvent(keyChar, keyState);
        injectEvent(e);
    }

    public void injectKeyClick(AutoKeyCode keyCode) {
        AutoEvent e;

        e = eventFactory.createKeyEvent(keyCode, AutoKeyState.PRESSED);
        injectEvent(e);

        e = eventFactory.createKeyEvent(keyCode, AutoKeyState.RELEASED);
        injectEvent(e);
    }
    
    public void injectKeyClick(char keyChar) {
        AutoEvent e;

        e = eventFactory.createKeyEvent(keyChar, AutoKeyState.PRESSED);
        injectEvent(e);

        e = eventFactory.createKeyEvent(keyChar, AutoKeyState.RELEASED);
        injectEvent(e);        
    }
    

    public void replayEvents(AutoEventSequence events, int speedDivisor) {
    }

    public void replayEvents(AutoEventSequence events) {
        replayEvents(events, 1);
    }

    private static native void getForegroundIsolateAndDisplay(
            int[] foregroundIsolateAndDisplay);
}
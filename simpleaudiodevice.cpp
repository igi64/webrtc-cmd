#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "simpleaudiodevice.h"
#include "webrtc/system_wrappers/interface/critical_section_wrapper.h"                                                                                        
#include "webrtc/system_wrappers/interface/trace.h"

namespace webrtc {

enum { kAdmMaxIdleTimeProcess = 1000 };

SimpleAudioDevice::SimpleAudioDevice() :
        critSect_(*CriticalSectionWrapper::CreateCriticalSection()),
        ptrCbAudioDeviceObserver_(NULL),
        ptrCbAudioTransport_(NULL),
        isRecording_(false),
        isPlayouting_(false) {
    
}

SimpleAudioDevice::~SimpleAudioDevice() {
    
}

int32_t SimpleAudioDevice::TimeUntilNextProcess() {
    return kAdmMaxIdleTimeProcess;
}

int32_t SimpleAudioDevice::Process() {
    return 0;
}

int32_t SimpleAudioDevice::RegisterEventObserver(AudioDeviceObserver* eventCallback) {
    CriticalSectionScoped lock(&critSect_);
    ptrCbAudioDeviceObserver_ = eventCallback;
    return 0;
}

int32_t SimpleAudioDevice::RegisterAudioCallback(AudioTransport* audioCallback) {
    CriticalSectionScoped lock(&critSect_);
    ptrCbAudioTransport_ = audioCallback;
    return 0;
}

void SimpleAudioDevice::Test() {
    static unsigned char buffer[40960];
    uint32_t samples = 0;
    uint32_t micLevel;
    int ret;

    int64_t elapsed_time_ms = -1;
    int64_t ntp_time_ms = -1;

    if ( ptrCbAudioTransport_ != NULL) {
        if ( isPlayouting_ ) {
            ret = ptrCbAudioTransport_->NeedMorePlayData(
                    160,
                    2,
                    1,
                    16000,
                    buffer,
                    samples,
                    &elapsed_time_ms,
                    &ntp_time_ms);
        }
        
        if ( isRecording_ ) {
            ret = ptrCbAudioTransport_->RecordedDataIsAvailable(
                    buffer,
                    160,
                    2,
                    1,
                    16000,
                    0,
                    0,
                    0,
                    0,
                    micLevel);
        }
    }    
    
}

}  // namespace webrtc

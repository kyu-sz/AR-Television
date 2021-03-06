////////////////////////////////////////////////////////////
/// AR Television
/// Copyright(c) 2017 Carnegie Mellon University
/// Licensed under The MIT License[see LICENSE for details]
/// Written by Kai Yu, Zhongxu Wang, Ruoyuan Zhao, Qiqi Xiao
////////////////////////////////////////////////////////////
#include <chrono>

#include <common/CVUtils.h>
#include <common/ErrorCodes.h>

using namespace std;
using namespace cv;

using timepoint = chrono::steady_clock::time_point;

namespace ar {
    void RealtimeLocalVideoStream::Restart() {
        start_time_ = chrono::steady_clock::now();
        frame_cnt_ = 0;
    }

    ERROR_CODE RealtimeLocalVideoStream::Open(const char *video_path) {
        cap_ = VideoCapture(video_path, cv::CAP_FFMPEG);
        if (!cap_.isOpened())
            return AR_FILE_NOT_FOUND;

        fps_ = cap_.get(CAP_PROP_FPS);

        return AR_SUCCESS;
    }

    ERROR_CODE RealtimeLocalVideoStream::NextFrame(Mat &output_buf) {
        if (!cap_.isOpened())
            return AR_UNINITIALIZED;

        auto now = chrono::steady_clock::now();
        int dest_frame_ind = int((now - start_time_).count() * fps_ / 1000000000);
        while (frame_cnt_++ < dest_frame_ind)
            cap_.grab();
        bool ret = cap_.retrieve(output_buf);
        if (!ret || output_buf.empty())
            return AR_NO_MORE_FRAMES;
        else
            return AR_SUCCESS;
    }

    void InterestPointsTracker::GenKeypointsDesc(const Mat &frame,
                                                 vector<KeyPoint> &keypoints,
                                                 Mat &descriptors) {
        detector_->detectAndCompute(frame, noArray(), keypoints, descriptors);
//        descriptors.convertTo(descriptors, CV_32F);
    }

    std::vector<std::pair<int, int>> InterestPointsTracker::MatchKeypoints(const cv::Mat &descriptors1,
                                                                           const cv::Mat &descriptors2) {
        std::vector<std::pair<int, int>> matches;
        vector<vector<DMatch>> dmatches;
        matcher_->knnMatch(descriptors1, descriptors2, dmatches, 2);
        for (auto &dmatch : dmatches)
            // We only use the matches where the best match is much better than the second best match.
            if (dmatch[0].distance < NN_MATCH_RATIO * dmatch[1].distance)
                matches.emplace_back(dmatch[0].queryIdx, dmatch[0].trainIdx);
        return matches;
    }

    InterestPointsTracker::InterestPointsTracker(cv::Ptr<cv::Feature2D> detector,
                                                 cv::Ptr<cv::DescriptorMatcher> matcher) :
            detector_(std::move(detector)),
            matcher_(std::move(matcher)) {}
}
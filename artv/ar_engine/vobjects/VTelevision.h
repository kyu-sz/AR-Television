///////////////////////////////////////////////////////////
// AR Television
// Copyright(c) 2017 Carnegie Mellon University
// Licensed under The MIT License[see LICENSE for details]
// Written by Kai Yu, Zhongxu Wang, Ruoyuan Zhao, Qiqi Xiao
///////////////////////////////////////////////////////////
#pragma once
#include <opencv2/opencv.hpp>

#include <common/CVUtils.h>
#include <ar_engine/VObject.h>

namespace ar
{
	class VTelevision : public VObject
	{
		FrameStream& content_stream_;
	public:
		VTelevision(AREngine& engine,
					int id,
					FrameStream& content_stream);

		void locate(const cv::Ptr<InterestPoint>& left_upper,
					const cv::Ptr<InterestPoint>& left_lower,
					const cv::Ptr<InterestPoint>& right_upper,
					const cv::Ptr<InterestPoint>& right_lower);

		inline virtual VObjType GetType() { return TV; }
		void Draw(cv::Mat& scene, const cv::Mat& camera_matrix);
	};
}

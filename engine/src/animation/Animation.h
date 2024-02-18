#ifndef __PIONNER_ANIMATION_ANIMATION_H__
#define __PIONNER_ANIMATION_ANIMATION_H__

#include "asset/Asset.h"
#include "Bone.h"

namespace pio
{
	class MeshSource;
	class Animator;
	class Skeleton;
	struct MeshNode;
	struct BoneInformation;

	class Animation
	{
	public:
		Animation(const std::string &name) : m_name(name) {}
		~Animation();

		void buildGPUSkinning(Ref<Skeleton> &skeleton, const std::vector<MeshNode> &hierarchy);
		Ref<Bone> findBone(const std::string &name);	

		void start();
		void pause();
		void stop();

	public:
		inline void insertBone(const Ref<Bone> &bone) { if (bone) { m_bones[bone->getName()] = bone; } }
		inline const std::string &getName() const { return m_name; }
		inline int32_t getTicksPerSec() const { return m_ticksPerSecond; }
		inline float getDuration() const { return m_duration; }

	private:
		void calcGPUSkinning(const std::map<std::string, BoneInformation> &boneInfo, const std::vector<MeshNode> &hierarchy, uint32_t index,
							 glm::mat4 &parentTransform, float curTick, FrameMats &frameMats);

	private:
		std::string m_name;
		/** Duration of the animation in ticks **/
		float m_duration{ 0.f };
		/** Ticks per second **/
		int32_t m_ticksPerSecond{ 0 };
		std::map<std::string, Ref<Bone>> m_bones;
		GPUSkinning m_gpuSkinning;

	private:
		friend class AssimpAnimationImporter;
		friend class Animator;
	};

	class AnimationAsset : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::Animation)
	public:
		AnimationAsset(const Ref<MeshSource> &meshSource);
		virtual ~AnimationAsset();

		inline const Ref<Animator> &getAnimator() const { return m_animator; }

	private:
		Ref<Animator> m_animator;
	};
}

#endif
#pragma once

#include "AoraDevice.h"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace aor
{
    class AoraDescriptorSetLayout {
    public:
        class Builder {
        public:
            Builder(AoraDevice& aoraDevice) : aoraDevice{ aoraDevice } {}

            Builder& addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<AoraDescriptorSetLayout> build() const;

        private:
            AoraDevice& aoraDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        AoraDescriptorSetLayout(
            AoraDevice& aoraDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~AoraDescriptorSetLayout();
        AoraDescriptorSetLayout(const AoraDescriptorSetLayout&) = delete;
        AoraDescriptorSetLayout& operator=(const AoraDescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        AoraDevice& aoraDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class AoraDescriptorWriter;
    };

    class AoraDescriptorPool {
    public:
        class Builder {
            public:
                Builder(AoraDevice& aoraDevice) : aoraDevice{ aoraDevice } {}

                Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
                Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
                Builder& setMaxSets(uint32_t count);
                std::unique_ptr<AoraDescriptorPool> build() const;

            private:
                AoraDevice& aoraDevice;
                std::vector<VkDescriptorPoolSize> poolSizes{};
                uint32_t maxSets = 1000;
                VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        AoraDescriptorPool(
            AoraDevice& aoraDevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~AoraDescriptorPool();
        AoraDescriptorPool(const AoraDescriptorPool&) = delete;
        AoraDescriptorPool& operator=(const AoraDescriptorPool&) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();

    private:
        AoraDevice& aoraDevice;
        VkDescriptorPool descriptorPool;

        friend class AoraDescriptorWriter;
    };

    class AoraDescriptorWriter {
    public:
        AoraDescriptorWriter(AoraDescriptorSetLayout& setLayout, AoraDescriptorPool& pool);

        AoraDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        AoraDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        AoraDescriptorSetLayout& setLayout;
        AoraDescriptorPool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };
}

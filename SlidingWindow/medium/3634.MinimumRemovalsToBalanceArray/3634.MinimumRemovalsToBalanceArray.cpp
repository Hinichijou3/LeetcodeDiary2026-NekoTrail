class Solution {
public:
    int minRemoval(vector<int>& nums, int k) {
        sort(nums.begin(),nums.end());
        int left=0;
        int length=0;
        for(int i=0;i<nums.size();i++)
        {
            while(1LL*nums[left]*k<nums[i])
            {
                left++;
            }
            length=max(length, i-left+1);
        }
        return nums.size()-length;
    }
};
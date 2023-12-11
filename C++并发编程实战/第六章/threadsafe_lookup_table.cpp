/*************************************************************************
	> File Name: threadsafe_lookup_table.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/12/11 13:51:12
 ************************************************************************/

#include <mutex>
#include <shared_mutex>
#include <vector>
#include <list>
#include <iterator>
#include <memory>
#include <ranges>
#include <map>

template <typename Key,typename Value,typename Hash=std::hash<Key>>
class threadsafe_lookup_table{
private:
	class bucket_type{
		friend class threadsafe_lookup_table;
	private:
		using bucket_value=std::pair<Key,Value>;
		using bucket_data=std::list<bucket_value>;
		using bucket_iterator=typename bucket_data::iterator;
		bucket_data data;
		mutable std::shared_mutex mutex;
		
		bucket_iterator find_entry_for(const Key& key){
			return std::ranges::find_if(data,[&](const bucket_value& item){
				return item.first==key;
			});
		}
	public:
		Value value_for(const Key& key,const Value& default_value){
			std::shared_lock<std::shared_mutex> lock(mutex);
			const bucket_iterator found_entry=find_entry_for(key);
			return (found_entry==data.end())?default_value:found_entry->second;
		}
		void add_or_update_mapping(const Key& key,const Value& value){
			std::unique_lock<std::shared_mutex> lock(mutex);
			auto found_entry=find_entry_for(key);
			if(found_entry==data.end()){
				data.push_back(bucket_value(key,value));
			}
			else{
				found_entry->second=value;
			}
		}
		void remove_mapping(const Key& key){
			std::unique_lock<std::shared_mutex> lock(mutex);
			auto found_entry=find_entry_for(key);
			if(found_entry!=data.end()){
				data.erase(found_entry);
			}
		}
	};
private:
	std::vector<std::unique_ptr<bucket_type>> buckets;
	Hash hasher;

	bucket_type& get_bucket(const Key& key)const{
		const std::size_t bucket_index=hasher(key)%buckets.size();
		return *(buckets[bucket_index]);
	}
public:
	threadsafe_lookup_table(unsigned num_buckets=19,const Hash& hasher_=Hash()):
		buckets(num_buckets),hasher(hasher_){
			for(unsigned i=0;i<num_buckets;++i){
				buckets[i].reset(new bucket_type);
			}
		}
	threadsafe_lookup_table(const threadsafe_lookup_table& other)=delete;
	threadsafe_lookup_table& operator=(const threadsafe_lookup_table& other)=delete;

	Value value_for(const Key& key,const Value& default_value=Value()){
		return get_bucket(key).value_for(key,default_value);
	}

	void add_or_update_mapping(const Key& key,const Value& value){
		return get_bucket(key).add_or_update_mapping(key,value);
	}

	void remove_mapping(const Key& key){
		return get_bucket(key).remove_mapping(key);
	}

	std::map<Key,Value> get_map(){
		std::vector<std::unique_lock<std::shared_mutex>> locks;
		for(const auto& bucket:buckets){
			locks.emplace_back(bucket->mutex);
		}
		std::map<Key,Value> res;
		for(const auto& bucket:buckets){
			res.insert(bucket->data.begin(),bucket->data.end());
		}
		return res;
	}
};

#include <iostream>
#include <thread>
void test(){
    threadsafe_lookup_table<int,int> table;
	std::thread t1([&table](){
		for(int i=50;i<100;++i){
			table.add_or_update_mapping(i,i);
		}
	});
	std::thread t3([&table](){
		for(int i=50;i<100;++i){
			auto ret=table.value_for(i,-1);
			if(ret!=-1){
				table.remove_mapping(i);
			}
			else{
				std::cout<<"not found\n";
			}
		}
	});
	t1.join();
	t3.join();
	auto res=table.get_map();
	for(const auto& item:res){
		std::cout<<item.first<<" "<<item.second<<std::endl;
	}
}

int main(){
    test();
	return 0;
}

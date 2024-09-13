#include "ae.hpp"

ArithmeticEncoding::ArithmeticEncoding(map<char, double> frequency_table, bool save_stages)
    : save_stages(save_stages) {
    if (save_stages) {
        cout << "WARNING: Setting save_stages=true may cause memory overflow if the message is large." << std::endl;
    }
    probability_table = get_probability_table(frequency_table);
}

map<char, double> ArithmeticEncoding::get_probability_table(const std::map<char, double>& frequency_table) {
    double total_frequency = 0;
    for (const auto& pair : frequency_table) {
        total_frequency += pair.second;
    }

    std::map<char, double> probability_table;
    for (const auto& pair : frequency_table) {
        probability_table[pair.first] = pair.second / total_frequency;
    }

    return probability_table;
}

tuple<long double, long double, long double> ArithmeticEncoding::get_encoded_value(const map<char, vector<long double>>& last_stage_probs) {
    std::vector<long double> last_stage_values;
    for (const auto& pair : last_stage_probs) {
        last_stage_values.insert(last_stage_values.end(), pair.second.begin(), pair.second.end());
    }

    long double last_stage_min = *min_element(last_stage_values.begin(), last_stage_values.end());
    long double last_stage_max = *max_element(last_stage_values.begin(), last_stage_values.end());
    long double encoded_value = (last_stage_min + last_stage_max) / 2;

    return std::make_tuple(last_stage_min, last_stage_max, encoded_value);
}

map<char, std::vector<long double>> ArithmeticEncoding::process_stage(const map<char, double>& probability_table, long double stage_min, long double stage_max) {
    map<char, std::vector<long double>> stage_probs;
    long double stage_domain = stage_max - stage_min;

    for (const auto& pair : probability_table) {
        long double term_prob = pair.second;
        long double cum_prob = term_prob * stage_domain + stage_min;
        stage_probs[pair.first] = { stage_min, cum_prob };
        stage_min = cum_prob;
    }

    return stage_probs;
}

tuple<long double, vector<map<char, vector<long double>>>, long double, long double> ArithmeticEncoding::encode(const string& msg, const map<char, double>& probability_table) {
    vector<map<char, vector<long double>>> encoder;

    long double stage_min = 0.0;
    long double stage_max = 1.0;

    for (char msg_term : msg) {
        auto stage_probs = process_stage(probability_table, stage_min, stage_max);

        stage_min = stage_probs[msg_term][0];
        stage_max = stage_probs[msg_term][1];

        if (save_stages) {
            encoder.push_back(stage_probs);
        }
    }

    auto last_stage_probs = process_stage(probability_table, stage_min, stage_max);
    if (save_stages) {
        encoder.push_back(last_stage_probs);
    }

    auto [interval_min_value, interval_max_value, encoded_msg] = get_encoded_value(last_stage_probs);

    return std::make_tuple(encoded_msg, encoder, interval_min_value, interval_max_value);
}

void ArithmeticEncoding::rescale(long double &stage_min, long double &stage_max) {
    long double range = stage_max - stage_min;
    stage_min = 0.0;
    stage_max = range;
}

tuple<string, vector<map<char, vector<long double>>>> ArithmeticEncoding::decode(long double encoded_msg, int msg_length, const map<char, double>& probability_table) {
    vector<map<char, vector<long double>>> decoder;
    string decoded_msg;

    long double stage_min = 0.0;
    long double stage_max = 1.0;

    for (int i = 0; i < msg_length; ++i) {
        auto stage_probs = process_stage(probability_table, stage_min, stage_max);

        char msg_term = 0;
        bool found = false;
        for (const auto& pair : stage_probs) {
            if (encoded_msg >= pair.second[0] && encoded_msg <= pair.second[1]) {
                msg_term = pair.first;
                found = true;
                break;
            }
        }

        if (!found) {
            break;
        }

        decoded_msg += msg_term;
        stage_min = stage_probs[msg_term][0];
        stage_max = stage_probs[msg_term][1];

        if (stage_max - stage_min < 1e-128) {
            rescale(stage_min, stage_max);
        }
    }

    return make_tuple(decoded_msg, decoder);
}


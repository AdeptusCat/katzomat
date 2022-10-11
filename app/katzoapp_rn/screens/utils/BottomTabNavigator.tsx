import {createBottomTabNavigator} from '@react-navigation/bottom-tabs';
import React from 'react';
import {Text, View, Button} from 'react-native';
import Icon from 'react-native-vector-icons/AntDesign';

export type BottomTabParamList = {
  HomeTab: undefined;
  SetttingsTab: undefined;
};

const Tab = createBottomTabNavigator<BottomTabParamList>();

const HomeTabScreen = ({navigation}) => {
  React.useEffect(() => {
    navigation.setOptions({
      headerTintColor: 'white',
      headerTitleStyle: {
        fontWeight: 'bold',
      },
      headerStyle: {
        backgroundColor: 'red',
      },
      headerRight: () => <Button title="new Color" color="black" />,
    });
  }, []);
  return (
    <View style={{flex: 1, justifyContent: 'center', alignItems: 'center'}}>
      <Text>Home Tab!</Text>
    </View>
  );
};

const SettingsTabScreen = () => {
  return (
    <View style={{flex: 1, justifyContent: 'center', alignItems: 'center'}}>
      <Text>Settings Tab!</Text>
    </View>
  );
};

export const BottomTabNavigator = () => {
  return (
    <Tab.Navigator
      screenOptions={{
        tabBarActiveTintColor: 'tomato',
        tabBarInactiveTintColor: 'blue',
      }}>
      <Tab.Screen
        name="HomeTab"
        component={HomeTabScreen}
        options={{
          tabBarBadge: 3,
          title: 'Home',
          tabBarIcon: ({size, color}) => <Icon name={'home'} size={20} />,
        }}
      />
      <Tab.Screen
        name="SettingsTab"
        component={SettingsTabScreen}
        options={{
          title: 'Settings',
          tabBarIcon: ({size, color}) => <Icon name={'setting'} size={20} />,
        }}
      />
    </Tab.Navigator>
  );
};

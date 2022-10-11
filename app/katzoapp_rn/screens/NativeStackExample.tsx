import {createNativeStackNavigator} from '@react-navigation/native-stack';
import React from 'react';
import {Text, Button} from 'react-native';
import {SafeAreaView} from 'react-native-safe-area-context';
import {GetRandomRgb} from './utils/GetRandomRgb.tsx';

export type StackParamList = {
  Home: undefined;
  Details: {
    radomColor: string;
  };
};

const Stack = createNativeStackNavigator<StackParamList>();

const HomeScreen = ({
  route,
  navigation,
}: NativeStackScreenProps<StackParamList, 'Home'>) => {
  return (
    <SafeAreaView
      style={{flex: 1, alignItems: 'center', justifyContent: 'center'}}>
      <Text>Home Screen</Text>
      <Button
        title="Go to Details"
        onPress={() => {
          navigation.navigate('Details', {
            randomColor: GetRandomRgb(),
          });
        }}
      />
    </SafeAreaView>
  );
};



const DetailsScreen = ({
  route,
  navigation,
}: NativeStackScreenProps<StackParamList, 'Details'>) => {
  const {randomColor} = route.params;

  const onUpdateColor = () => {
    const newColor = GetRandomRgb();
    navigation.setOptions({
      headerTitle: newColor,
      headerStyle: {
        backgroundColor: newColor,
      },
    });
  };
  React.useEffect(() => {
    navigation.setOptions({
      headerTintColor: 'white',
      headerTitleStyle: {
        fontWeight: 'bold',
      },
      headerStyle: {
        backgroundColor: randomColor,
      },
      headerRight: () => (
        <Button title="new Color" color="black" onPress={onUpdateColor} />
      ),
    });
  }, []);
  return (
    <SafeAreaView
      style={{flex: 1, alignItems: 'center', justifyContent: 'center'}}>
      <Text>Details Screen</Text>
      <Button title="Go Back" onPress={() => navigation.goBack()} />
      <Button title="Go Home" onPress={() => navigation.navigate('Home')} />
      <Button
        title="Go back to first screen in stack"
        onPress={() => navigation.popToTop()}
      />
      <Button
        title="Update rgb"
        onPress={() => {
          navigation.setOptions({
            headerTitle: randomColor,
            title: randomColor,
            headerStyle: {
              backgroundColor: randomColor,
            },
          });
        }}
      />
      <Text>RGB received: {randomColor}</Text>
    </SafeAreaView>
  );
};

export const NativeStackExample = () => {
  return (
    <Stack.Navigator initialRouteName="Home">
      <Stack.Screen
        name="Home"
        component={HomeScreen}
        options={{title: 'Overview'}}
      />
      <Stack.Screen
        name="Details"
        component={DetailsScreen}
        options={{title: 'Details'}}
      />
    </Stack.Navigator>
  );
};
